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
