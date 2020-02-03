# 配置相关

1. 日志模块的配置
```
log:
    level: DEBUG
    appender:
        - ConsoleAppender
        - FileAppender
            file: 1.log
        - FileAppender
            file: 2.log
```
