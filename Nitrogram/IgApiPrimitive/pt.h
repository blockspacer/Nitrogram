#ifndef __PROTOTHREAD_H__
#define __PROTOTHREAD_H__

class Protothread {
public:
	Protothread() : _ptLine(0) { }

	void Restart() { _ptLine = 0; }

	void Stop() { _ptLine = LineNumberInvalid; }

	bool IsRunning() { return _ptLine != LineNumberInvalid; }

protected:
	typedef unsigned short LineNumber;
	static const LineNumber LineNumberInvalid = (LineNumber)(-1);

	LineNumber _ptLine;
};

#define PT_BEGIN() bool ptYielded = true; switch (_ptLine) { case 0:
#define PT_END() default: ; } Stop(); return false;
#define PT_WAIT_UNTIL(condition)                        \
            do { _ptLine = __LINE__; case __LINE__:     \
            if (!(condition)) return true; } while (0)
#define PT_WAIT_WHILE(condition) PT_WAIT_UNTIL(!(condition))
#define PT_RESTART() do { Restart(); return true; } while (0)
#define PT_EXIT() do { Stop(); return false; } while (0)
#define PT_YIELD()                                                      \
            do { ptYielded = false; _ptLine = __LINE__; case __LINE__:  \
            if (!ptYielded) return true; } while (0)
#define PT_YIELD_UNTIL(condition)                                       \
            do { ptYielded = false; _ptLine = __LINE__; case __LINE__:  \
            if (!ptYielded || !(condition)) return true; } while (0)

#endif // __PROTOTHREAD_H__
