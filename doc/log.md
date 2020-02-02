# 日志模块(流式日志)

1. 宏定义
- LOG(level): 输出对应级别的日志。
- LOG_DEBUG: 输出 debug 级别的日志，相当于 LOG(DEBUG)。
- LOG_INFO: 输出 info 级别的日志，相当于 LOG(INFO)。
- LOG_WARN: 输出 warn 级别的日志，相当于 LOG(WARN)。
- LOG_ERROR: 输出 error 级别的日志，相当于 LOG(ERROR)。
- LOG_FATAL: 输出 fatal 级别的日志，相当于 LOG(FATAL)。
```
LOG(DEBUG) << "LOG" << "(" << "DEBUG" << ")";
LOG_DEBUG << "LOG" << "(" << "DEBUG" << ")";

LOG(INFO) << "LOG" << "(" << "INFO" << ")";
LOG_INFO << "LOG" << "(" << "INFO" << ")";

LOG(WARN) << "LOG" << "(" << "WARN" << ")";
LOG_WARN << "LOG" << "(" << "WARN" << ")";

LOG(ERROR) << "LOG" << "(" << "ERROR" << ")";
LOG_ERROR << "LOG" << "(" << "ERROR" << ")";

LOG(FATAL) << "LOG" << "(" << "FATAL" << ")";
LOG_FATAL << "LOG" << "(" << "FATAL" << ")";
```

2. 日志类型(Level)
- 作用: 记录日志级别。
- 类型:
    - UNKNOWN(0): 未知类型
    - ALL(1)    : 打开所有日志记录
    - DEBUG(2)  : 调试信息
    - INFO(3)   : 运行信息
    - WARN(4)   : 警告信息
    - ERROR(5)  : 错误信息
    - FATAL(6)  : 严重错误信息
    - OFF(7)    : 关闭所有日志记录
- 描述:    
    1. UNKNOWN 一般不使用，当用户设定的类型不存在时内部使用该类型进行表示。
    2. ALL/OFF 表示打开或关闭所有的日志输出。
    3. 使用 LOG(level) 时，请不要设定为 UNKNOWN/ALL/OFF 类型。
    4. 当日志输出级别设定为 X 级别时，仅输出大于等于该级别的日志信息。例如，当输出级别被设定为 ERROR 级别时，仅输出 ERROR/FATAL 级别的信息，DEBUG/INFO/WARN 级别的信息将不会被输出。

2. 日志事件(Event)
- 作用: 记录日志的相关信息。
- 描述:
    1. 当析构函数被调用时输出日志。

3. 日志格式控制器(Layout)
- 作用: 调整日志输出格式。
- 描述: 暂无。 

4. 日志输出地(Appender)
- 作用: 调整日志输出地点，可以将日志输出至控制台或文件。
- 描述:
    1. 该类有两个派生类(ConsoleAppender/FileAppender)，分别表示将日志输出到控制台和将日志输出到文件。 

5. 日志(Logger)
- 作用: 控制日志级别以及输出地。 
- 描述: 
    1. 单例模式。
    2. 可以配置多个输出地。
