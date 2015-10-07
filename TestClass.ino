class RunningAverage
{
public:
    RunningAverage(void);
    RunningAverage(int);
    ~RunningAverage();

    void clear();
    void addValue(int);
    void fillValue(int, int);

    int getAverage();

    int getElement(uint8_t idx);
    uint8_t getSize() { return _size; }
    uint8_t getCount() { return _cnt; }

protected:
    uint8_t _size;
    uint8_t _cnt;
    uint8_t _idx;
    int   _sum;
    int * _ar;
};

RunningAverage::RunningAverage(int n)
{
    _size = n;
    _ar = (int*) malloc(_size * sizeof(int));
    if (_ar == NULL) _size = 0;
    clear();
}

RunningAverage::~RunningAverage()
{
    if (_ar != NULL) free(_ar);
}

// resets all counters
void RunningAverage::clear()
{
    _cnt = 0;
    _idx = 0;
    _sum = 0.0;
    for (int i = 0; i< _size; i++) _ar[i] = 0.0;  // needed to keep addValue simple
}

// adds a new value to the data-set
void RunningAverage::addValue(int f)
{
    if (_ar == NULL) return;
    _sum -= _ar[_idx];
    _ar[_idx] = f;
    _sum += _ar[_idx];
    _idx++;
    if (_idx == _size) _idx = 0;  // faster than %
    if (_cnt < _size) _cnt++;
}

// returns the average of the data-set added sofar
int RunningAverage::getAverage()
{
    if (_cnt == 0) return NAN;
    return _sum / _cnt;
}

// returns the value of an element if exist, 0 otherwise
int RunningAverage::getElement(uint8_t idx)
{
    if (idx >=_cnt ) return NAN;
    return _ar[idx];
}

// fill the average with a value
// the param number determines how often value is added (weight)
// number should preferably be between 1 and size
void RunningAverage::fillValue(int value, int number)
{
    clear();
    for (int i = 0; i < number; i++)
    {
        addValue(value);
    }
}

class A
{
  public:
   int x;
   virtual void f() { x=1; }
};

class B : public A
{
  public:
    int y;
    virtual void f() { x=2; }
};


A *a;
B *b;
const int TEST_PIN = 10;

void setupTestClass()
{
   a=new A(); 
   b=new B();
   pinMode(TEST_PIN,OUTPUT);
}

void loopTestLoop()
{
   a->f();
   b->f();
   digitalWrite(TEST_PIN,(a->x == b->x) ? HIGH : LOW);
}
