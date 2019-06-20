#!/bin/sh

samples=(`find build  -name "test_sample_*"`) # 将找到的结果保存为一个数组需要加 ()   

for sample in $samples; do
    ./${sample}
done
