# 配置相关

1. 日志模块的配置
```
log:
    level: "DEBUG"
    appender:
        - type: "ConsoleAppender"
          pattern: "default"
        - type: "FileAppender"
          pattern: "default"
          file: "/reps/seda/log/1.log"
        - type: "FileAppender"
          pattern: "default"
          file: "/reps/seda/log/2.log"
```

2. Stage 模块的配置
```
# 假设现有一个任务，描述如下:
# 给定一个字符串
# 1. 统计每个字母的出现频率(CountStage)
# 2. 寻找出现频率最高的字母和出现频率最低的字母(MaxMinStage)
# 3. 计算其 ASCII 码的乘积(MultiStage)
# 4. 将结果转换为二进制(BinaryStage)

# 假设每个 Stage 有 3 种状态: 处理成功(Success)、处理失败(Failure)、等待处理(Waiting)

# 配置如下

stage:
    CountStage: 1
        Success: MaxMinStage
        Failure: CountStage
        Waiting: CountStage
    MaxMinStage: 3
        Success: MultiStage
        Failure: MaxMinStage
        Waiting: MaxMinStage
    MultiStage: 5
        Success: BinaryStage
        Failure: MultiStage
        Waiting: MultiStage
    BinaryStage: 7
```
