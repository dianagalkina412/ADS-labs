#include <iostream>
#include <random>
#include <stdexcept>

using namespace std;

// ---------- Узел списка ----------
template <typename T>
struct Node {
    T _data;
    Node* _next;

    Node(const T& data, Node* next = nullptr)
        : _data(data), _next(next) {
    }
};

// ---------- Односвязный список ----------
template <typename T>
class LinkedList {
private:
    Node<T>* _head;
    Node<T>* _tail;
    size_t _size;

    // Очистка списка
    void clear() {
        while (_head) {
            Node<T>* node = _head;
            _head = _head->_next;
            delete node;
        }

        _tail = nullptr;
        _size = 0;
    }

    // Рекурсивная функция вывода списка на печать в обратном порядке.
    void print_reverse_helper(Node<T>* node) const {
        if (node) {
            print_reverse_helper(node->_next);
            cout << node->_data << ' ';
        }
    }

public:
    // Конструктор по умолчанию
    LinkedList()
        : _head(nullptr), _tail(nullptr), _size(0) {}

    // Конструктор копирования
    LinkedList(const LinkedList& other)
        : _head(nullptr), _tail(nullptr), _size(0) {
        for (Node<T>* node = other._head; node; node = node->_next) {
            push_tail(node->_data);
        }
    }

    // Конструктор со случайными значениями: n элементов в интервале [low, high], зерно seed
    LinkedList(size_t n, const T& low, const T& high, unsigned seed)
        : _head(nullptr), _tail(nullptr), _size(0) {
        mt19937 gen(seed);
        uniform_int_distribution<T> dist(low, high);
        for (size_t i = 0; i < n; i++) {
            push_tail(dist(gen));
        }
    }

    // Деструктор
    ~LinkedList() {
        clear();
    }

    // Операция присваивания
    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            clear();
            for (Node<T>* node = other._head; node; node = node->_next) {
                push_tail(node->_data);
            }
        }

        return *this;
    }

    size_t get_size() const {
        return _size;
    }

    bool is_empty() const {
        return _size == 0;
    }

    // Добавление элемента в конец
    void push_tail(const T& data) {
        Node<T>* node = new Node<T>(data);
        if (!_head) {
            _head = _tail = node;
        }
        else {
            _tail->_next = node;
            _tail = node;
        }

        _size++;
    }

    // Добавление другого списка в конец (в исходном порядке)
    void push_tail(const LinkedList& other) {
        for (Node<T>* node = other._head; node; node = node->_next) {
            push_tail(node->_data);
        }
    }

    // Добавление элемента в начало
    void push_head(const T& data) {
        _head = new Node<T>(data, _head);
        if (!_tail) {
            _tail = _head;
        }

        _size++;
    }

    // Добавление другого списка в начало (порядок списка сохраняется):
    // копируем его узлы и целиком пришиваем перед головой
    void push_head(const LinkedList& other) {
        Node<T>* copy_head = nullptr;
        Node<T>* copy_tail = nullptr;
        for (Node<T>* node = other._head; node; node = node->_next) {
            Node<T>* copy_node = new Node<T>(node->_data);
            if (!copy_head) {
                copy_head = copy_tail = copy_node;
            }
            else {
                copy_tail->_next = copy_node;
                copy_tail = copy_node;
            }
        }

        if (copy_head) {
            copy_tail->_next = _head;
            _head = copy_head;
            if (!_tail) {
                _tail = copy_tail;
            }

            _size += other._size;
        }
    }

    // Удаление элемента из начала
    T pop_head() {
        if (is_empty()) {
            throw underflow_error("pop_head: список пуст");
        }

        Node<T>* node = _head;
        T data = node->_data;
        _head = _head->_next;
        if (!_head) {
            _tail = nullptr;
        }

        delete node;
        _size--;
        return data;
    }

    // Удаление элемента из конца
    T pop_tail() {
        if (is_empty()) {
            throw underflow_error("pop_tail: список пуст");
        }

        T data = _tail->_data;
        if (_head == _tail) {
            delete _head;
            _head = _tail = nullptr;
        }
        else {
            Node<T>* node = _head;
            while (node->_next != _tail) {
                node = node->_next;
            }

            node->_next = nullptr;
            delete _tail;
            _tail = node;
        }

        _size--;
        return data;
    }

    // Удаление всех узлов с заданным значением
    void delete_node(const T& data) {
        Node<T>* node = _head;
        Node<T>* prev_node = nullptr;
        while (node) {
            if (node->_data == data) {
                Node<T>* found_node = node;
                node = node->_next;
                if (prev_node) {
                    prev_node->_next = node;
                }
                else {
                    _head = node;
                }

                if (found_node == _tail) {
                    _tail = prev_node;
                }

                delete found_node;
                _size--;
            }
            else {
                prev_node = node;
                node = node->_next;
            }
        }
    }

    // Доступ по индексу: запись
    T& operator[](size_t index) {
        if (index >= _size) {
            throw out_of_range("LinkedList: индекс вне диапазона");
        }

        Node<T>* node = _head;
        for (size_t i = 0; i < index; i++) {
            node = node->_next;
        }

        return node->_data;
    }

    // Доступ по индексу: чтение
    const T& operator[](size_t index) const {
        if (index >= _size) {
            throw out_of_range("LinkedList: индекс вне диапазона");
        }

        Node<T>* node = _head;
        for (size_t i = 0; i < index; i++) {
            node = node->_next;
        }

        return node->_data;
    }

    // Печать в обратном порядке (рекурсивно)
    void print_reverse() const {
        print_reverse_helper(_head);
        cout << endl;
    }

    // Печать списка
    template <typename U>
    friend ostream& operator<<(ostream& os, const LinkedList<U>& list);
};

// Печать списка
template <typename T>
ostream& operator<<(ostream& os, const LinkedList<T>& list) {
    os << '[';
    for (Node<T>* node = list._head; node; node = node->_next) {
        os << node->_data;
        if (node->_next) {
            os << " -> ";
        }
    }

    os << ']';
    return os;
}

// ---------- Ханойская башня ----------
// Стержень — это LinkedList: голова списка = вершина башни,
// поэтому push_head/pop_head работают как "положить/снять диск".
// Рисуем три башни: '##' — диски, '||' — стержень
void print_towers(const LinkedList<int>* pegs, int n) {
    int width = 2 * n + 2;

    for (int p = 0; p < 3; p++) {                            // стержни
        for (int i = 0; i < width; i++) {
            cout << (i == width / 2 - 1 || i == width / 2 ? '|' : ' ');
        }

        cout << "  ";
    }

    cout << endl;
    for (int level = n - 1; level >= 0; level--) {           // башни
        for (int p = 0; p < 3; p++) {
            int disk = 0;                                    // 0 = нет диска
            if (level < static_cast<int>(pegs[p].get_size())) {
                disk = pegs[p][pegs[p].get_size() - 1 - level];
            }

            if (disk == 0) {                                 // стержень
                for (int i = 0; i < width; i++) {
                    cout << (i == width / 2 - 1 || i == width / 2 ? '|' : ' ');
                }
            }
            else {                                           // диск
                int left = (width - 2 * disk) / 2;
                for (int i = 0; i < width; i++) {
                    cout << (i >= left && i < left + 2 * disk ? '#' : ' ');
                }
            }

            cout << "  ";
        }

        cout << endl;
    }

    for (int p = 0; p < 3; p++) {                            // основание
        for (int i = 0; i < width; i++) {
            cout << '=';
        }

        cout << "  ";
    }

    cout << endl;
    for (int p = 0; p < 3; p++) {                            // подписи
        for (int i = 0; i < width / 2 - 1; i++) {
            cout << ' ';
        }

        cout << static_cast<char>('A' + p);
        for (int i = 0; i < width - width / 2; i++) {
            cout << ' ';
        }

        cout << "  ";
    }

    cout << endl << endl;
}

// Перемещение диска от одной башни к другой
void move_disk(LinkedList<int>* pegs, int from, int to) {
    if (pegs[from].is_empty()) {
        throw underflow_error("move_disk: стержень-источник пуст");
    }

    if (!pegs[to].is_empty() && pegs[to][0] < pegs[from][0]) {
        throw logic_error("move_disk: невозможно разместить диск большего размера на диске меньшего размера");
    }

    pegs[to].push_head(pegs[from].pop_head());
}

// Рекуривный алгоритм перемещения дисков с выводом результатов каждого хода
void hanoi(LinkedList<int>* pegs, int height, int n, int from, int to, int aux, long long& counter) {
    if (n > 0) {
        hanoi(pegs, height, n - 1, from, aux, to, counter);
        move_disk(pegs, from, to);
        counter++;
        cout << "Ход №" << counter << ": диск " << n << "  " << static_cast<char>('A' + from)
            << " -> " << static_cast<char>('A' + to) << endl;
        print_towers(pegs, height);
        hanoi(pegs, height, n - 1, aux, to, from, counter);
    }
}

// ---------- Демонстрация ----------
int main() {
    // Случайный список: 10 элементов из интервала [-5, 5], seed = 42
    LinkedList<int> list(10, -5, 5, 42);
    cout << "Случайный список:                       " << list << endl;
    cout << "Обратный список (print_reverse):        ";
    list.print_reverse();

    // Копирование и присваивание
    LinkedList<int> copy(list);
    cout << "Конструктор копирования:                " << copy << endl;
    LinkedList<int> assigned;
    assigned = list;
    cout << "Оператор присваивания:                  " << assigned << endl;

    // push/pop
    list.push_tail(100);
    list.push_head(-100);
    cout << "После push_tail(100) и push_head(-100): " << list << endl;

    // Дополнительный случайный список: 3 элемента из интервала [0, 1], seed = 7
    LinkedList<int> extra(3, 0, 1, 7);
    cout << "Дополнительный список:                  " << extra << endl;
    list.push_tail(extra);
    cout << "После push_tail(extra):                 " << list << endl;
    list.push_head(extra);
    cout << "После push_head(extra):                 " << list << endl;

    cout << "pop_head() = " << list.pop_head() << ", pop_tail() = " << list.pop_tail() << endl;
    cout << "Результат:                              " << list << endl;

    // Удаление всех узлов со значением
    int del = list[0];
    cout << "Чтение по индексу list[0] = " << del << endl;
    list.delete_node(del);
    cout << "После delete_node(" << del << "):                   " << list << endl;

    // Доступ по индексу и ошибка
    try {
        list[0] = 999;
        cout << "Запись по индексу list[0] = 999:        " << list << endl;
        cout << list.get_size() + 1 << "-ый элемент: ";
        cout << list[list.get_size() +1] << endl;   // кинет out_of_range
    }
    catch (const exception& e) {
        cout << "Перехваченное исключение: " << e.what() << endl;
    }

    // Ханойская башня на трёх списках
    const int height = 4; // высота башни (количество дисков)
    LinkedList<int> pegs[3]; // 3 башни-списка
    for (int d = 1; d <= height; d++) {
        pegs[0].push_tail(d);   // A: [1 -> 2 -> 3 -> 4], голова = вершина башни
    }

    cout << endl << "=== Ханойская башня (высота башни: " << height << ") ===" << endl;
    print_towers(pegs, height);
    long long moves = 0;
    hanoi(pegs, height, height, 0, 2, 1, moves);
    cout << "Общее количество ходов: " << moves << " (2^Высота - 1 = " << (1 << height) - 1 << ")" << endl;

    return 0;
}
