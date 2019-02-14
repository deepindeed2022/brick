#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <pthread.h>
#include <signal.h>

#include <curl/curl.h>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

vector<string> globalCompletedQuerys;

string globalHost               = "localhost";
string globalPort               = "6351";
string globalQueryFile          = "query.txt";
volatile bool globalStop        = false;

static int  globalThreadNumber  = 3;
static int  globalConcurrence   = 1;
static long globalQueryNumber   = 1;

void sigHandle(int)
{
    globalStop = true;
    cout << "stop "<< endl;
}

class statisticsRequest
{
    private:
        int failedRequest;
        int successRequest;
    public:
        statisticsRequest()
        {
            failedRequest = 0;
            successRequest = 0; 
        }
        void increaseFailedRequest(){ failedRequest++; }
        void increaseSucessRequest(){ successRequest++; }
        int getFailedRequest(){ return failedRequest; }
        int getSuccessRequest(){ return successRequest; }
};

struct threadArg
{
    int  sequence;
    int  concurrence;
    int  totalQueryNumber;
    statisticsRequest * statsReq;
    threadArg():sequence(-1), concurrence(1), totalQueryNumber(0), statsReq(NULL){}
};

size_t writeResponse(void *contents, size_t size, size_t nmemb, void *stream)
{
    string data((const char *)contents, (size_t)size * nmemb);
    statisticsRequest *request = (statisticsRequest *)stream;

    if(data.substr(0, 9) == "rsp_ec: 0")
    {
        request->increaseSucessRequest();
    }
    else if(data.substr(0, 6) == "rsp_ec" && data.substr(0, 9) != "rsp_ec: 0")
    {
        request->increaseFailedRequest();
    }

    return size * nmemb;
}

void getCompletedQuery(const string& queryContext, string& completedQuery)
{

    completedQuery.clear();//注意初始化数组
    completedQuery += "http://";
    completedQuery +=  globalHost;
    completedQuery += ":";
    completedQuery += globalPort;
    completedQuery += "/mod_qsrch/warmup?kw=";
    completedQuery +=  queryContext;

    return;
}

void setCurlEasy(CURL *curl, const string &completedQuery, statisticsRequest *response)
{
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L); 
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(curl, CURLOPT_URL, completedQuery.c_str());
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, writeResponse);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void *)response);

    return;
}

bool asyncSetCurlEasy(const vector<string>& completedQuerys, 
                      int queryBeginPosition,
                      size_t queryNumber,
                      CURLM *curlm,
                      statisticsRequest* statsReq)
{
    if(completedQuerys.empty() || statsReq == NULL || curlm == NULL)
        return false;

    for(size_t i = 0; !globalStop && i < queryNumber; ++i)
    {
        CURL *curl = curl_easy_init(); 
        if(curl)
        {
            setCurlEasy(curl, completedQuerys[queryBeginPosition + i], statsReq);
            curl_multi_add_handle(curlm, curl); 
        }
        else
            return false;
    }

    return true;
}

void asyncDealWithCurlCode(CURLM *curlm)
{
    if(curlm == NULL) 
        return;

    int leftMsg = 0;
    CURLMsg* msg = NULL;

    while(!globalStop && (msg = curl_multi_info_read(curlm, &leftMsg)) != NULL)
    {
        if(msg->msg == CURLMSG_DONE) 
        {
            int httpStatusCode = 0; 
            curl_easy_getinfo(msg->easy_handle, CURLINFO_RESPONSE_CODE, &httpStatusCode);

            char *effectiveUrl = NULL;
            curl_easy_getinfo(msg->easy_handle, CURLINFO_EFFECTIVE_URL, &effectiveUrl); 
            //cout << "url: " << effectiveUrl << " status:  " << httpStatusCode << "  " << curl_easy_strerror(msg->data.result)  << endl;         
        }
    }

    return;
}

void splitQuery(const vector<string> &queryContexts, vector<string> &completedQuerys)
{
    for(vector<string>::const_iterator it = queryContexts.begin(); it !=queryContexts.end(); ++it)
    {
        completedQuerys.push_back("");
        getCompletedQuery(*it,completedQuerys.back());  
    }

}

void asyncSendRequestAndGetResponse(const vector<string>& completedQuerys, 
                            int sequence,
                            int queryBeginPosition, 
                            int concur,
                            int queryNumber,
                            statisticsRequest *statsReq)
{
    CURLM *curlm = curl_multi_init();
    if(curlm == NULL) 
    {
        cout << "init curl multi failed" << endl; 
        return;
    }

    if(concur > queryNumber)
        concur = queryNumber;

    int runningCurls = 0, queryStart = queryBeginPosition; 
    int queryEndPosition = queryBeginPosition + queryNumber;
    int sendedRequest = queryBeginPosition - queryStart;
    int receivedResponse = statsReq->getFailedRequest() + statsReq->getSuccessRequest();

    asyncSetCurlEasy(completedQuerys, queryBeginPosition, concur, curlm, statsReq);
    queryBeginPosition += concur;

    do{
        curl_multi_perform(curlm, &runningCurls);
        asyncDealWithCurlCode(curlm); 

        sendedRequest = queryBeginPosition - queryStart;
        receivedResponse = statsReq->getFailedRequest() + statsReq->getSuccessRequest();

        if(queryBeginPosition != queryEndPosition && sendedRequest - receivedResponse < concur)
        {
            int curlNumberToAdd = concur - (sendedRequest - receivedResponse);

            if(queryBeginPosition + curlNumberToAdd < queryEndPosition)
            {
                asyncSetCurlEasy(completedQuerys, queryBeginPosition, curlNumberToAdd, curlm, statsReq);
                queryBeginPosition += curlNumberToAdd;
            }
            else if(queryBeginPosition < queryEndPosition)
            {
                asyncSetCurlEasy(completedQuerys, queryBeginPosition, queryEndPosition - queryBeginPosition, curlm, statsReq);
                queryBeginPosition += queryEndPosition - queryBeginPosition;
            }
        }

        curl_multi_wait(curlm, NULL, 0, 200000, NULL);  

        //cout << pthread_self() << " receivedResponse: " << receivedResponse << "queryNumber :" << queryNumber << endl;  
    }while(receivedResponse < queryNumber && !globalStop);

    curl_multi_cleanup(curlm); 

    return;
}

int getQueryContext(const string& path, vector<string>& queryContexts, int num)
{
    int index = 0;
    string line;
    ifstream in(path.c_str());
    while(!globalStop && index < num && (in >> line)){
        if(line.empty())
            continue;
        queryContexts.push_back(line);
        index++;
    }
    in.close();

    return index;
}

void *doWarmBySendQueryFormFile(void *arg)
{
    struct threadArg *queryMsg = (threadArg*)arg; 
    int averageQueryNumber = queryMsg->totalQueryNumber / globalThreadNumber;
    int queryBeginPosition = queryMsg->sequence * averageQueryNumber;
    if(queryMsg->sequence == globalThreadNumber - 1) 
        averageQueryNumber = queryMsg->totalQueryNumber - averageQueryNumber * queryMsg->sequence;
    //cout << "thread " << queryMsg->sequence << " query begin position is " << queryBeginPosition << " query number is " << averageQueryNumber << "concurrence is " << queryMsg->concurrence << endl;
     asyncSendRequestAndGetResponse(globalCompletedQuerys, queryMsg->sequence, queryBeginPosition, queryMsg->concurrence,  averageQueryNumber, queryMsg->statsReq);

    return NULL;
}

void usage(const char* pname)
{
    cout << pname << 
           "-p port "
           "-h host "
           "-i input_file "
           "-t thread_number" << endl;
}

void *timeWorker(void *arg)
{
    pthread_detach(pthread_self());
    struct timespec delay;
    delay.tv_sec = 10 * 60;
    delay.tv_nsec = 0;

    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask, SIGALRM);
    pthread_sigmask(SIG_BLOCK, &mask, NULL);

    nanosleep(&delay, NULL);
    globalStop = true;
    cout << "time to exit" << endl;

    return NULL;
}

bool parseParameters(int argc, char *argv[])
{
    int c;
    while((c = getopt(argc, argv, "h:p:i:t:n:c:")) != -1)
    {
        switch(c)
        {
            case 'p': 
                {
                    string port(optarg);
                    globalPort = port;
                }
                break;
            case 'h':
                {
                    string host(optarg);
                    globalHost = host;
                }
                break;
            case 'i':
                {
                    string queryFile(optarg);
                    globalQueryFile = queryFile;
                }
                break;
            case 't':
                globalThreadNumber = atoi(optarg); 
                break;
            case 'n':
                globalQueryNumber = atoi(optarg);
                break;
            case 'c':
                globalConcurrence = atoi(optarg);
                break;
            default:
                usage(argv[0]);
                return false;
        } 
    }

    return true;
}

void getStatisticsRequest(statisticsRequest statsReqs[], int *totalSuccessRequest, int *totalFailedRequest)
{
    for(int i = 0; i < globalThreadNumber; i++)
    {
        *totalSuccessRequest += statsReqs[i].getSuccessRequest();
        *totalFailedRequest  += statsReqs[i].getFailedRequest(); 
    }
}

int main(int argc, char* argv[])
{
    CURLcode code = curl_global_init(CURL_GLOBAL_ALL); 
    if(code != CURLE_OK)
    {
        cout << "curl_global_init error" << endl; 
        exit(-1);
    }

    if(!parseParameters(argc, argv))
    {
        cout << "parse parameters error" << endl;
        exit(-1);
    }

    signal(SIGINT, sigHandle);
    signal(SIGTERM, sigHandle);

    vector<string> queryContexts;
    int queryNumber = getQueryContext(globalQueryFile, queryContexts, globalQueryNumber); 
    if(!queryNumber)
    {
        cout << "read query from file failed" << endl;
        exit(-1);
    }
    splitQuery(queryContexts, globalCompletedQuerys);

    pthread_t *tids = new pthread_t[globalThreadNumber];
    if(tids == NULL)
    {
        cout << "new pthread failed" << endl;
        exit(-1);
    }

    threadArg *thr_args = new threadArg[globalThreadNumber]();
    if(thr_args == NULL)
    {
        cout << "new pthread arg  failed"<< endl;
        exit(-1);
    }

    statisticsRequest *statsReqs = new statisticsRequest[globalThreadNumber]();
    if(statsReqs == NULL) 
    {
        cout << "new statsReqs arg  failed"<< endl;
        exit(-1);
    }

    for(int i = 0; i < globalThreadNumber; ++i)
    {
        thr_args[i].sequence = i;
        thr_args[i].statsReq = &statsReqs[i];
        thr_args[i].concurrence = globalConcurrence;
        thr_args[i].totalQueryNumber = globalQueryNumber;

        if(pthread_create(&tids[i], NULL, doWarmBySendQueryFormFile, &thr_args[i]) != 0)    
        {
            cout << "create curl thread" << i << " error" << endl; 
            exit(-1);
        } 
    }

    pthread_t twid;
    if(pthread_create(&twid, NULL, timeWorker, NULL) != 0)
    {
        cout << "create time worker thread error"<< endl; 
        exit(-1);
    }

    for(int i = 0; i < globalThreadNumber; ++i)
    {
        pthread_join(tids[i], NULL);
    }

    int totalSuccessRequest = 0;
    int totalFailedRequest  = 0;
    getStatisticsRequest(statsReqs, &totalSuccessRequest, &totalFailedRequest);
    cout << "total request : " << totalSuccessRequest + totalFailedRequest << endl
         << "totalSuccessRequest is " << totalSuccessRequest << endl
         << "totalFailedRequest is " << totalFailedRequest << endl;

    delete[] statsReqs; 
    delete[] thr_args;
    delete[] tids;

    curl_global_cleanup();
    return 0;
}