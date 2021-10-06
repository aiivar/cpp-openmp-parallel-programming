#include <iostream>
#include <map>
#include <omp.h>

class Strategy {
public:
    ~Strategy() = default;
    Strategy() = default;

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
    void execute() override {

#pragma omp parallel num_threads(8)
        printf("-- Hello world. Thread #%d, treads count %d --\n", omp_get_thread_num(), omp_get_num_threads());

    }
};

class OpenMPTask_2 : public Strategy {
public:
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
            printf("-- Hello world. Thread #%d, treads count %d. Region 1 --\n", omp_get_thread_num(),
                   omp_get_num_threads());
        }
        omp_set_num_threads(n2);
#pragma omp parallel if(omp_get_max_threads() > nf2)
        {
            printf("-- Hello world. Thread #%d, treads count %d. Region 2 --\n", omp_get_thread_num(),
                   omp_get_num_threads());
        }

    }
};

class OpenMPTask_3 : public Strategy {
public:
    void execute() override {

        int a, b;
        a = 10;
        b = 20;

        printf("---- Before: a = %d, b = %d ----\n", a, b);

#pragma omp parallel num_threads(2) private(a) firstprivate(b)
        {
            a = 0;
            a += omp_get_thread_num();
            b += omp_get_thread_num();
            printf("-- Thread #%d Region 1: a = %d, b = %d --\n", omp_get_thread_num(), a, b);
        }
        printf("---- After Region 1: a = %d, b = %d ----\n", a, b);
#pragma omp parallel num_threads(4) shared(a) private(b)
        {
            b = 0;
#pragma omp atomic
            a -= omp_get_thread_num();
            b -= omp_get_thread_num();
            printf("-- Thread #%d Region 2: a = %d, b = %d --\n", omp_get_thread_num(), a, b);
        }
        printf("---- After Region 2: a = %d, b = %d ----\n", a, b);

    }
};

class OpenMPTask_4 : public Strategy {
public:
    void execute() override {

        int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int b[10] = {100, 29, 3218, 327, 3216, 255, 64, 663, 862, 1};

        int targetA = 10;
        int targetB = 1;

#pragma omp parallel sections num_threads(2)
        {
#pragma omp section
            {
                for (int i = 0; i < 10; ++i) {
                    if (targetA > a[i]) {
                        targetA = a[i];
                    }
                }
            }
#pragma omp section
            {
                for (int i = 0; i < 10; ++i) {
                    if (targetB < b[i]) {
                        targetB = b[i];
                    }
                }
            }
        }

        printf("Min a[] = %d, max b[] = %d\n", targetA, targetB);
    }
};

class OpenMPTask_5 : public Strategy {
public:
    void execute() override {

        int d[6][8];
        int max_range = 10;

        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j < 8; ++j) {
                d[i][j] = rand() % max_range;
            }
        }
        for (int i = 0; i < 6; ++i) {
            printf("| ");
            for (int j = 0; j < 8; ++j) {
                printf("%d ", d[i][j]);
            }
            printf("|\n");
        }
        printf("\n");

#pragma omp parallel sections
        {
#pragma omp section
            {
                int sum = 0;
                int count = 0;
                for (int i = 0; i < 6; ++i) {
                    for (int j = 0; j < 8; ++j) {
                        sum += d[i][j];
                        count++;
                    }
                }
                float n = sum * 1.0 / count;
                printf("-- Среднее арифмитическое = %.4f (Tread #%d)\n", n, omp_get_thread_num());
            }
#pragma omp section
            {
                int min = max_range;
                int max = -1;
                for (int i = 0; i < 6; ++i) {
                    for (int j = 0; j < 8; ++j) {
                        if (max < d[i][j]){
                            max = d[i][j];
                        }
                        if (min > d[i][j]){
                            min = d[i][j];
                        }
                    }
                }
                printf("-- Максимальное значение = %d (Tread #%d)\n-- Минимальное значение = %d (Tread #%d)\n", max, omp_get_thread_num(), min, omp_get_thread_num());
            }
#pragma omp section
            {
                int count = 0;
                int divider = 3;
                for (int i = 0; i < 6; ++i) {
                    for (int j = 0; j < 8; ++j) {
                        if (d[i][j] % divider == 0){
                            count++;
                        }
                    }
                }
                printf("-- Количество элементов кратных трем = %d (Tread #%d)\n", count, omp_get_thread_num());
            }
        }
    }
};

class OpenMPTask_6 : public Strategy{
public:
    void execute() override {
        const int n = 100;
        int a[n];
        int max_range = 100;
        for (int i = 0; i < n; ++i) {
            a[i] = rand() % max_range;
        }
        int sum = 0;
#pragma omp parallel
        {
            //TODO 6th task
        };
    }
};

class OpenMPTask_7 : public Strategy{
public:
    void execute() override {
        const int n = 12;
        int a[n], b[n], c[n];

#pragma omp parallel num_threads(3)
        {
#pragma omp for schedule(static)
            for (int i = 0; i < n; ++i) {
                a[i] = rand() % n;
                b[i] = rand() % n;
                printf("- Thread %d of %d\n", omp_get_thread_num()+1, omp_get_num_threads());
            }
        }

        printf("a: ");
        for (int i = 0; i < n; ++i) {
            printf("%d ", a[i]);
        }
        printf("\n");
        printf("b: ");
        for (int i = 0; i < n; ++i) {
            printf("%d ", b[i]);
        }
        printf("\n");

#pragma omp parallel num_threads(4)
        {
#pragma omp for schedule(dynamic)
            for (int i = 0; i < n; ++i) {
                c[i] = a[i] + b[i];
                printf("- Thread %d of %d\n", omp_get_thread_num()+1, omp_get_num_threads());
            }
        }

        printf("c: ");
        for (int i = 0; i < n; ++i) {
            printf("%d ", c[i]);
        }
        printf("\n");
    }
};

class OpenMPTask_8 : public Strategy{
public:
    void execute() override {
        const int n = 16;
        int a[n];
        double b[n];
        for (int i = 0; i < n; ++i) {
            a[i] = i;
        }
#pragma omp parallel for schedule(static) num_threads(8) shared(b)
        for (int i = 1; i < n - 1; i++) {
            b[i] = (a[i - 1] + a[i] + a[i + 1]) * 1.0 / 3;
            printf("%.0f:%d ", b[i], omp_get_thread_num());
            if (i == n - 2) printf("\n");
        }

#pragma omp parallel for schedule(dynamic) num_threads(8) shared(b)
        for (int i = 1; i < n - 1; i++) {
            b[i] = (a[i - 1] + a[i] + a[i + 1]) * 1.0 / 3;
            printf("%.0f:%d ", b[i], omp_get_thread_num());
            if (i == n - 2) printf("\n");
        }

#pragma omp parallel for schedule(dynamic, 3) num_threads(8) shared(b)
        for (int i = 1; i < n - 1; i++) {
            b[i] = (a[i - 1] + a[i] + a[i + 1]) * 1.0 / 3;
            printf("%.0f:%d ", b[i], omp_get_thread_num());
            if (i == n - 2) printf("\n");
        }
        printf("\n");
    }
};

class OpenMPTask_9 : public Strategy{
public:
    void execute() override {
        //TODO 9th
    }
};

class OpenMPTask_10 : public Strategy{
public:
    void execute() override {
        //TODO 10th
    }
};

class OpenMPTask_11 : public Strategy{
public:
    void execute() override {
        //TODO 11th
    }
};

class OpenMPTask_12 : public Strategy{
public:
    void execute() override {
        //TODO 12th
    }
};

class OpenMPTask_13 : public Strategy{
public:
    void execute() override {
        //TODO 13th
    }
};

std::map<int, Strategy *> &getMap(std::map<int, Strategy *> &taskMapping);

int main() {
    std::map<int, Strategy *> taskMapping;
    taskMapping = getMap(taskMapping);

    int task;

    printf("## Enter number of the task:");

    scanf("%d", &task);

    if (task < 1 || task > taskMapping.size()) {
        return 0;
    }

    Context *context = new Context();
    context->setStrategy(taskMapping[task]);
    context->runStrategy();
    printf("## End of task #%d", task);

    //end
    return 0;
}

std::map<int, Strategy *> &getMap(std::map<int, Strategy *> &taskMapping) {
    taskMapping[1] = new OpenMPTask_1();
    taskMapping[2] = new OpenMPTask_2();
    taskMapping[3] = new OpenMPTask_3();
    taskMapping[4] = new OpenMPTask_4();
    taskMapping[5] = new OpenMPTask_5();
    taskMapping[6] = new OpenMPTask_6();
    taskMapping[7] = new OpenMPTask_7();
    taskMapping[8] = new OpenMPTask_8();
    taskMapping[9] = new OpenMPTask_9();
    taskMapping[10] = new OpenMPTask_10();
    taskMapping[11] = new OpenMPTask_11();
    taskMapping[12] = new OpenMPTask_11();
    taskMapping[13] = new OpenMPTask_13();
    return taskMapping;
}
