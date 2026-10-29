#include <iostream>
#include <cstdlib>
#include <random> 
#include <complex> 
using namespace std;

// Обобщенный класс для реализации многочлена:
template<int power, class T> class Polynom {
private:
    // Массив с коэффициентами (размер массива на единицу больше степени многочлена):
    T a[power + 1];

public:
    // Конструктор без аргументов:
    Polynom() {
        // Заполнение массива нулями:
        for (int k = 0; k <= power; k++) {
            a[k] = 0;
        }
    }

    // Конструктор с аргументом - именем числового массива:
    Polynom(T* b) {
        // Копирование элементов из одного массива в другой:
        for (int k = 0; k <= power; k++) {
            a[k] = b[k];
        }
    }

    // Метод для для установки коэффициента при заданной степени:
    void set(int k, T value) {
        if (k >= 0 && k <= power) {
            a[k] = value;
        }
    }

    // Метод для отображения значений элементов поля - массива:
    void print() {
        for (int k = 0; k <= power; k++) {
            cout << a[k] << " | ";
        }

        cout << endl;
    }

    // Обобщенный операторный метод, уменьшающий степень многочлена до известного значения
    template<int new_power, class T> Polynom<new_power, T> compress() {
        // Локальный объект обобщенного класса:
        Polynom<new_power, T> res;
        // Вычисление коэффициентов для многочлена-результата:
        for (int k = 0; k <= new_power; k++) {
            res[k] = a[k];
        }

        return res;
    }

    template<int power, class T> int maxNonZeroIndex() {
        for (int i = power; i > 0; i--) {
            if (a[i] != (T)0) {
                return i;
            }
        }
        
        return 1;
    }

    template<int power, class T> Polynom<maxNonZeroIndex(), T> shrink_to_fit() {
        // Локальный объект обобщенного класса:
        Polynom<maxNonZeroIndex(), T> res;
        return res.compress();
    }

    // Операторный метод позволяет вызвать объект.
    // Результатом возвращается значение многочлена для переданного аргумента:
    T operator()(T x) {
        T s = 0, q = 1;
        for (int k = 0; k <= power; k++) {
            s += a[k] * q;
            q *= x;
        }

        return s;
    }

    // Операторный метод для индексирования объектов:
    T& operator[](int k) {
        return a[k];
    }

    // Обобщенный операторный метод для вычисления произведения многочленов:
    template<int n, class T> Polynom<power + n, T> operator*(Polynom<n, T> pol) {
        // Локальный объект обобщенного класса:
        Polynom<power + n, T> res;
        // Вычисление коэффициентов для многочлена - результата:
        for (int i = 0; i <= power; i++) {
            for (int j = 0; j <= n; j++) {
                res[i + j] += a[i] * pol[j];
            }
        }

        return res;
    }

    // Обобщенный операторный метод для вычисления суммы многочленов:
    template<int n, class T> Polynom<(n > power ? n : power), T> operator+(Polynom<n, T> pol) {
        // Локальный объект обобщенного класса:
        Polynom<(n > power ? n : power), T> res;
        // Вычисление коэффициентов для многочлена-результата:
        for (int i = 0; i <= power; i++) {
            res[i] += a[i];
        }

        for (int i = 0; i <= n; i++) {
            res[i] += pol[i];
        }

        return res;
    }
};

// Обобщенная операторная функция для вычисления результата умножения многочлена на число:
template<int power, class T> Polynom<power, T> operator*(Polynom<power, T> pol, T r) {
    // Локальный объект обобщенного класса:
    Polynom<power, T> res;
    // Вычисление коэффициентов для многочлена-результата:
    for (int k = 0; k <= power; k++) {
        res[k] = pol[k] * r;
    }

    return res;
}

// Обобщенная операторная функция для вычисления результата умножения числа на многочлен:
template<int power, class T> Polynom<power, T> operator*(T r, Polynom<power, T> pol) {
    // Многочлен умножается на число:
    return pol * r;
}

// Обобщенная операторная функция для вычисления разности двух многочленов:
template<int m, int n, class T> Polynom<(m > n ? m : n), T> operator-(Polynom<m, T> x, Polynom<n, T> y) {
    // К первому многочлену прибавляется второй, умноженный на число -1:
    return x + ((T) -1) * y;
}

// Обобщенный операторный метод, резервирующий память под более высокие степени с коэффициентом 0
template<int power, class T> Polynom<power + 1, T> expand(Polynom<power, T> pol) {
    // Локальный объект обобщенного класса:
    Polynom<power + 1, T> res;
    // Вычисление коэффициентов для многочлена-результата:
    for (int k = 0; k <= power; k++) {
        res[k] = pol[k];
    }

    res[power + 1] = (T)0;
    return res;
}

// Обобщенный операторный метод, уменьшающий степень многочлена
template<int power, class T> Polynom<power - 1, T> compress(Polynom<power, T> pol) {
    // Локальный объект обобщенного класса:
    Polynom<power - 1, T> res;
    // Вычисление коэффициентов для многочлена-результата:
    for (int k = 0; k < power; k++) {
        res[k] = pol[k];
    }

    return res;
}

// Обобщенная функция для вычисления производной от многочлена:
template<int power, class T> Polynom<power - 1, T> differential(Polynom<power, T> pol) {
    // Локальный объект обобщенного класса:
    Polynom<power - 1, T> res;
    // Вычисление коэффициентов для многочлена-результата:
    for (int k = 0; k <= power - 1; k++) {
        res[k] = pol[k + 1] * (T)(k + 1);
    }

    return res;
}

// Обобщенная функция для теста многочлена заданного типа:
template<int power_1, int power_2, class T> void test_polynom(Polynom<power_1, T> P1, Polynom<power_2, T> P2, T d) {
    cout << "=====================" << endl;
    // Первый многочлен:
    cout << "Многочлен P1 (размерность " << power_1 << "):\t";
    // Коэффициенты первого многочлена:
    P1.print();
    cout << "Значение P1(" << d << ") = ";
    // Значение первого многочлена в точке:
    cout << P1(d) << endl;
    cout << "Многочлен P1' (размерность " << power_1 - 1 << "):\t";
    // Коэффициенты для производной от многочлена:
    differential(P1).print();
    cout << "Значение P1'(" << d << ") = ";
    // Значение производной в точке:
    cout << differential(P1)(d) << endl;
    // Второй многочлен:
    cout << "Многочлен P2 (размерность " << power_2 << "):\t";
    // Коэффициенты второго многочлена:
    P2.print();
    cout << "Значение P2(" << d << ") = ";
    // Значение второго многочлена в точке:
    cout << P2(d) << endl;
    cout << "Многочлен P1*P2:\t";
    // Коэффициенты для произведения многочленов:
    (P1 * P2).shrink_to_fit().print();
    cout << "Значение (P1*P2)(" << d << ") = ";
    // Значение произведения многочленов в точке:
    cout << (P1 * P2)(d) << endl;
    cout << "Многочлен P1+P2:\t";
    // Коэффициенты для суммы многочленов:
    (P1 + P2).shrink_to_fit().print();
    cout << "Значение (P1+P2)(" << d << ") = ";
    // Значение суммы многочленов в точке:
    cout << (P1 + P2)(d) << endl;
    cout << "Многочлен P2-P1:\t";
    // Коэффициенты для разности многочленов:
    (P2 - P1).shrink_to_fit().print();
    cout << "Значение (P2-P1)(" << d << ") = ";
    // Значение разности многочленов в точке:
    cout << (P2 - P1)(d) << endl;
    cout << endl;
    cout << "=====================" << endl;
}

// Главная функция программы:
int main() {
    const int power_I1 = 12;
    const int power_I2 = 3;
    const int power_F1 = 11;
    const int power_F2 = 4;
    const int power_D1 = 10;
    const int power_D2 = 5;
    const int power_CF1 = 9;
    const int power_CF2 = 6;
    const int power_CD1 = 8;
    const int power_CD2 = 7;
    random_device rd;
    mt19937 g(rd());
    uniform_int_distribution<int> int_dist(-100, 100);
    uniform_real_distribution<float> float_dist(-100.0f, 100.0f);
    uniform_real_distribution<double> double_dist(-100.0, 100.0);

    int* I1 = new int[power_I1 + 1];
    for (int i = 0; i <= power_I1; i++) {
        I1[i] = int_dist(g);
    }

    Polynom<power_I1, int> PI1(I1);

    int* I2 = new int[power_I2 + 1];
    for (int i = 0; i <= power_I2; i++) {
        I2[i] = int_dist(g);
    }

    Polynom<power_I2, int> PI2(I2);
    cout << "Тест многочленов с типом 'int':" << endl;
    test_polynom(PI1, PI2, int_dist(g));

    float* F1 = new float[power_F1 + 1];
    for (int i = 0; i <= power_F1; i++) {
        F1[i] = float_dist(g);
    }

    Polynom<power_F1, float> PF1(F1);

    float* F2 = new float[power_F2 + 1];
    for (int i = 0; i <= power_F2; i++) {
        F2[i] = float_dist(g);
    }

    Polynom<power_F2, float> PF2(F2);
    cout << "Тест многочленов с типом 'float':" << endl;
    test_polynom(PF1, PF2, float_dist(g));

    double* D1 = new double[power_D1 + 1];
    for (int i = 0; i <= power_D1; i++) {
        D1[i] = double_dist(g);
    }

    Polynom<power_D1, double> PD1(D1);

    double* D2 = new double[power_D2 + 1];
    for (int i = 0; i <= power_D2; i++) {
        D2[i] = double_dist(g);
    }

    Polynom<power_D2, double> PD2(D2);
    cout << "Тест многочленов с типом 'double':" << endl;
    test_polynom(PD1, PD2, double_dist(g));

    complex<float>* CF1 = new complex<float>[power_CF1 + 1];
    for (int i = 0; i <= power_CF1; i++) {
        CF1[i] = complex<float>(float_dist(g), float_dist(g));
    }

    Polynom<power_CF1, complex<float>> PCF1(CF1);

    complex<float>* CF2 = new complex<float>[power_CF2 + 1];
    for (int i = 0; i <= power_CF2; i++) {
        CF2[i] = complex<float>(float_dist(g), float_dist(g));
    }

    Polynom<power_CF2, complex<float>> PCF2(CF2);
    cout << "Тест многочленов с типом 'complex<float>':" << endl;
    test_polynom(PCF1, PCF2, complex<float>(float_dist(g), float_dist(g)));

    complex<double>* CD1 = new complex<double>[power_CD1 + 1];
    for (int i = 0; i <= power_CD1; i++) {
        CD1[i] = complex<double>(double_dist(g), double_dist(g));
    }

    Polynom<power_CD1, complex<double>> PCD1(CD1);

    complex<double>* CD2 = new complex<double>[power_CD2 + 1];
    for (int i = 0; i <= power_CD2; i++) {
        CD2[i] = complex<double>(double_dist(g), double_dist(g));
    }

    Polynom<power_CD2, complex<double>> PCD2(CD2);
    cout << "Тест многочленов с типом 'complex<double>':" << endl;
    test_polynom(PCD1, PCD2, complex<double>(double_dist(g), double_dist(g)));

    return 0;
}