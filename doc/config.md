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
    CountStage: 
        max_thread: 1
        state:
            success: MaxMinStage
            failure: CountStage
            waiting: CountStage
    MaxMinStage: 
        max_thread: 3 
        state:
            success: MultiStage
            failure: MaxMinStage
            waiting: MaxMinStage
    MultiStage:
        max_thread: 5
        state:
            success: BinaryStage
            failure: MultiStage
            waiting: MultiStage
    BinaryStage: 
        max_thread: 7
        state: ~
```
