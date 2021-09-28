#include <iostream>
#include <map>
#include <omp.h>

class Strategy {
public:
    virtual ~Strategy() = default;

    virtual void execute() = 0;
};

class Context {
private:
    Strategy *strategy_;

public:
    Context(Strategy *strategy = nullptr) : strategy_(strategy) {

    }

    ~Context() {
        delete this->strategy_;
    }

    void setStrategy(Strategy *strategy) {
        delete this->strategy_;
        this->strategy_ = strategy;
    }

    void runStrategy() {
        this->strategy_->execute();
    }
};

class OpenMPTask_1 : public Strategy {
public:

    ~OpenMPTask_1() override = default;

    OpenMPTask_1() = default;

    void execute() override {
        #pragma omp parallel num_threads(8)
            printf("-- Hello world. Thread #%d, treads count %d --\n", omp_get_thread_num(), omp_get_num_threads());
    }
};

class OpenMPTask_2 : public Strategy {
public:

    OpenMPTask_2() = default;
    ~OpenMPTask_2() override = default;

    void execute() override {
        int n1, n2;
        n1 = 3;
        n2 = 2;
        int nf1 = 2;
        int nf2 = 2;
        omp_set_dynamic(0);
        omp_set_num_threads(n1);
        #pragma omp parallel if(omp_get_max_threads() > nf1)
        {
            printf("-- Hello world. Thread #%d, treads count %d. Region 1 --\n", omp_get_thread_num(), omp_get_num_threads());
        }
        omp_set_num_threads(n2);
        #pragma omp parallel if(omp_get_max_threads() > nf2)
        {
            printf("-- Hello world. Thread #%d, treads count %d. Region 2 --\n", omp_get_thread_num(), omp_get_num_threads());
        }
    }
};

class OpenMPTask_3 : public Strategy{
public:
    OpenMPTask_3() = default;
    ~OpenMPTask_3() override = default;

    void execute() override {
        int a, b;
        a = 0;
        b = 0;

        printf("-- Before: a = %d, b = %d --\n", a, b);

        #pragma omp parallel num_threads(2) private(a) firstprivate(b)
        {
            a = 0;
            a += omp_get_thread_num();
            b += omp_get_thread_num();
            printf("-- Region 1: a = %d, b = %d --\n", a, b);
        }

        #pragma omp parallel num_threads(4) shared(a) private(b)
        {
            b = 0;
            #pragma omp atomic
            a -= omp_get_thread_num();
            b -= omp_get_thread_num();
            printf("-- Region 2: a = %d, b = %d --\n", a, b);
        }
        printf("-- After: a = %d, b = %d --\n", a, b);
    }
};

int main() {
    std::map<int, Strategy *> taskMapping;
    taskMapping[1] = new OpenMPTask_1();
    taskMapping[2] = new OpenMPTask_2();
    taskMapping[3] = new OpenMPTask_3();

    int task;

    scanf("%d", &task);

    if (task < 1 || task > taskMapping.size()) {
        return 0;
    }

    Context *context = new Context();
    context->setStrategy(taskMapping[task]);
    context->runStrategy();

    //end
    return 0;
}
