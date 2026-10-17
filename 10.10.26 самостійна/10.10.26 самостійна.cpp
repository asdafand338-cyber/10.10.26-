10.10.26 самостійна
#include <iostream>
#include <stdexcept>
#include <utility>

template <typename T>
class Vector {
private:
    T* data;
    size_t size;
    size_t capacity;

    // Допоміжний метод для збільшення місткості
    void reserve(size_t newCapacity) {
        if (newCapacity <= capacity) return;

        T* newData = new T[newCapacity];
        for (size_t i = 0; i < size; ++i) {
            newData[i] = std::move(data[i]);
        }

        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    // Конструктор
    explicit Vector(size_t initialCapacity = 4)
        : size(0), capacity(initialCapacity > 0 ? initialCapacity : 4) {
        data = new T[capacity];
    }

    // Деструктор
    ~Vector() {
        delete[] data;
    }

    // Конструктор копіювання
    Vector(const Vector& other) : size(other.size), capacity(other.capacity) {
        data = new T[capacity];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    // Оператор присвоєння копіюванням
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            capacity = other.capacity;
            data = new T[capacity];
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    // Геттери та перевірка порожнечі
    size_t getSize() const { return size; }
    size_t getCapacity() const { return capacity; }
    bool empty() const { return size == 0; }

    // Додавання елемента в кінець (для зручності тестування)
    void pushBack(const T& value) {
        insertAt(static_cast<int>(size), value);
    }

    // ====================================================
    // 1. ВСТАВКА ЗА ІНДЕКСОМ (insertAt)
    // ====================================================
    void insertAt(int index, const T& value) {
        // Перевірка валідності індексу: від 0 до size включно
        if (index < 0 || index > static_cast<int>(size)) {
            std::cerr << "Помилка (insertAt): Невалідний індекс " << index
                << "! Допустимий діапазон: [0, " << size << "].\n";
            return;
        }

        // Автоматична зміна розміру, якщо не вистачає capacity
        if (size >= capacity) {
            reserve(capacity * 2);
        }

        // Зсув елементів вправо від кінця масиву до index
        for (size_t i = size; i > static_cast<size_t>(index); --i) {
            data[i] = std::move(data[i - 1]);
        }

        // Вставка елемента та збільшення size
        data[index] = value;
        ++size;
    }

    // ====================================================
    // 2. ВИДАЛЕННЯ ЗА ІНДЕКСОМ (removeAt)
    // ====================================================
    void removeAt(int index) {
        // Перевірка валідності індексу: від 0 до size - 1
        if (index < 0 || index >= static_cast<int>(size)) {
            std::cerr << "Помилка (removeAt): Невалідний індекс " << index
                << "! Допустимий діапазон: [0, " << (size > 0 ? size - 1 : 0) << "].\n";
            return;
        }

        // Зсув елементів вліво після index
        for (size_t i = static_cast<size_t>(index); i < size - 1; ++i) {
            data[i] = std::move(data[i + 1]);
        }

        // Зменшення size на 1
        --size;
    }

    // ====================================================
    // 3. ОЧИЩЕННЯ МАСИВУ (clear)
    // ====================================================
    void clear() {
        // Звільняти пам'ять не потрібно — просто скидаємо size
        size = 0;
    }

    // ====================================================
    // 4. ПЕРЕВАНТАЖЕННЯ ВИВОДУ В ПОТІК (operator<<)
    // ====================================================
    friend std::ostream& operator<<(std::ostream& os, const Vector<T>& arr) {
        // Крайній випадок: порожній масив
        if (arr.empty()) {
            os << "{}";
            return os;
        }

        os << "{ ";
        for (size_t i = 0; i < arr.size; ++i) {
            os << arr.data[i];
            // Без зайвої коми після останнього елемента
            if (i < arr.size - 1) {
                os << ", ";
            }
        }
        os << " }";

        return os;
    }
};

// ==========================================
// ТЕСТУВАННЯ (MAIN)
// ==========================================
int main() {
    std::cout << "=== 1. СТВОРЕННЯ ТА ДОДАВАННЯ ЕЛЕМЕНТІВ ===\n";
    Vector<int> arr;
    arr.pushBack(10);
    arr.pushBack(20);
    arr.pushBack(30);

    std::cout << "Початковий масив: " << arr << "\n\n";

    std::cout << "=== 2. ТЕСТУВАННЯ insertAt ===\n";
    // Вставка на початок (індекс 0)
    arr.insertAt(0, 5);
    std::cout << "Після insertAt(0, 5):  " << arr << "\n";

    // Вставка в середину (індекс 2)
    arr.insertAt(2, 15);
    std::cout << "Після insertAt(2, 15): " << arr << "\n";

    // Вставка в кінець (індекс = size)
    arr.insertAt(static_cast<int>(arr.getSize()), 40);
    std::cout << "Після insertAt(5, 40): " << arr << "\n";

    // Некоректний індекс
    arr.insertAt(100, 999);
    std::cout << "\n";

    std::cout << "=== 3. ТЕСТУВАННЯ removeAt ===\n";
    // Видалення з середини
    arr.removeAt(2); // Видаляємо 15
    std::cout << "Після removeAt(2): " << arr << "\n";

    // Видалення першого елемента
    arr.removeAt(0); // Видаляємо 5
    std::cout << "Після removeAt(0): " << arr << "\n";

    // Некоректний індекс
    arr.removeAt(-1);
    std::cout << "\n";

    std::cout << "=== 4. ТЕСТУВАННЯ clear ТА ПОРОЖНЬОГО ВИВОДУ ===\n";
    arr.clear();
    std::cout << "Масив після clear(): " << arr << "\n";
    std::cout << "Перевірка empty(): " << (arr.empty() ? "Так (true)" : "Ні (false)") << "\n";

    return 0;
}
10.10.26 практика
#include <iostream>
#include <utility>

template <typename T>
class Array {
private:
    T* data;
    size_t size;
    size_t capacity;

    // Допоміжний метод для збільшення місткості
    void reserve(size_t newCapacity) {
        if (newCapacity <= capacity) return;

        T* newData = new T[newCapacity];
        for (size_t i = 0; i < size; ++i) {
            newData[i] = std::move(data[i]);
        }

        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    // Конструктор
    explicit Array(size_t initialCapacity = 4)
        : size(0), capacity(initialCapacity > 0 ? initialCapacity : 4) {
        data = new T[capacity];
    }

    // Деструктор
    ~Array() {
        delete[] data;
    }

    // Конструктор копіювання
    Array(const Array& other) : size(other.size), capacity(other.capacity) {
        data = new T[capacity];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    // Оператор присвоєння
    Array& operator=(const Array& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            capacity = other.capacity;
            data = new T[capacity];
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    // Геттери
    size_t getSize() const { return size; }
    size_t getCapacity() const { return capacity; }

    // ====================================================
    // 1. ПЕРЕВІРКА НА ПОРОЖНЕЧУ (empty)
    // ====================================================
    bool empty() const {
        return size == 0;
    }

    // ====================================================
    // 2. ДОДАВАННЯ НА ПОЧАТОК (pushFront)
    // ====================================================
    void pushFront(const T& value) {
        // Якщо немає вільного місця — збільшуємо місткість удвічі
        if (size == capacity) {
            reserve(capacity * 2);
        }

        // Зсуваємо всі існуючі елементи вправо, починаючи з кінця
        for (size_t i = size; i > 0; --i) {
            data[i] = std::move(data[i - 1]);
        }

        // Записуємо нове значення в індекс 0 та збільшуємо size
        data[0] = value;
        ++size;
    }

    // ====================================================
    // 3. ВИДАЛЕННЯ З ПОЧАТКУ (popFront)
    // ====================================================
    void popFront() {
        // Захист: якщо масив порожній — нічого не робимо
        if (empty()) {
            std::cout << "[Попередження]: Спроба викликати popFront() на порожньому масиві!\n";
            return;
        }

        // Зсуваємо всі елементи, починаючи з індексу 1, вліво
        for (size_t i = 0; i < size - 1; ++i) {
            data[i] = std::move(data[i + 1]);
        }

        // Зменшуємо size на 1
        --size;
    }

    // Перевантаження виводу для зручної перевірки
    friend std::ostream& operator<<(std::ostream& os, const Array<T>& arr) {
        if (arr.empty()) {
            os << "{}";
            return os;
        }

        os << "{ ";
        for (size_t i = 0; i < arr.size; ++i) {
            os << arr.data[i];
            if (i < arr.size - 1) {
                os << ", ";
            }
        }
        os << " }";

        return os;
    }
};

// ==========================================
// ТЕСТУВАННЯ (MAIN)
// ==========================================
int main() {
    std::cout << "=== 1. ПЕРЕВІРКА empty() ТА ПОРОЖНЬОГО МАСИВУ ===\n";
    Array<int> arr;
    std::cout << "Початковий стан: " << arr << "\n";
    std::cout << "arr.empty(): " << (arr.empty() ? "true" : "false") << "\n\n";

    // Спроба викликати popFront() на порожньому масиві
    arr.popFront();
    std::cout << "\n";

    std::cout << "=== 2. ТЕСТУВАННЯ pushFront() ===\n";
    arr.pushFront(30);
    std::cout << "Після pushFront(30): " << arr << "\n";

    arr.pushFront(20);
    std::cout << "Після pushFront(20): " << arr << "\n";

    arr.pushFront(10);
    std::cout << "Після pushFront(10): " << arr << "\n";
    std::cout << "arr.empty(): " << (arr.empty() ? "true" : "false") << "\n\n";

    std::cout << "=== 3. ТЕСТУВАННЯ popFront() ===\n";
    arr.popFront(); // Видалить 10
    std::cout << "Після першого popFront(): " << arr << "\n";

    arr.popFront(); // Видалить 20
    std::cout << "Після другого popFront(): " << arr << "\n";

    arr.popFront(); // Видалить 30
    std::cout << "Після третього popFront(): " << arr << "\n";
    std::cout << "arr.empty(): " << (arr.empty() ? "true" : "false") << "\n";

    return 0;
}
11.10.26 самостійна
#include <iostream>
#include <string>


void parseFilePath(const std::string & fullPath) {
    std::cout << "Повний шлях: " << fullPath << "\n";

    
    size_t lastSlash = fullPath.rfind('\\');
    if (lastSlash == std::string::npos) {
        lastSlash = fullPath.rfind('/'); 
    }

   
    std::string fileNameWithExt;
    if (lastSlash != std::string::npos) {
        fileNameWithExt = fullPath.substr(lastSlash + 1);
    }
    else {
        fileNameWithExt = fullPath;
    }

    
    size_t lastDot = fileNameWithExt.rfind('.');

    std::string fileName;
    std::string extension;

    if (lastDot != std::string::npos && lastDot != 0) {
        fileName = fileNameWithExt.substr(0, lastDot);
        extension = fileNameWithExt.substr(lastDot + 1);
    }
    else {
        fileName = fileNameWithExt;
        extension = "немає розширення";
    }

  
    std::cout << "Чисте ім'я файлу: " << fileName << "\n";
    std::cout << "Розширення:       " << extension << "\n";
}


int countWordOccurrences(const std::string& text, const std::string& target) {
    if (target.empty()) return 0;

    int count = 0;
    size_t pos = 0;

    while ((pos = text.find(target, pos)) != std::string::npos) {
        ++count;
        
        pos += target.length();
    }

    return count;
}


int main() {
    std::cout << "=========================================\n";
    std::cout << "        ЗАВДАННЯ 1: АНАЛІЗ ШЛЯХУ         \n";
    std::cout << "=========================================\n";

    std::string path1 = "C:\\Users\\Admin\\Documents\\report.pdf";
    parseFilePath(path1);

    std::cout << "\n-----------------------------------------\n";
    std::string path2 = "D:\\Projects\\CPP\\main.cpp";
    parseFilePath(path2);

    std::cout << "\n=========================================\n";
    std::cout << "     ЗАВДАННЯ 2: ПІДРАХУНОК СЛІВ          \n";
    std::cout << "=========================================\n";

    std::string text = "C++ є чудовою мовою. Вивчення C++ відкриває нові можливості, адже C++ швидкий!";
    std::string word = "C++";

    std::cout << "Текст для пошуку:\n\"" << text << "\"\n\n";
    std::cout << "Шукаємо слово: \"" << word << "\"\n";

    int occurrences = countWordOccurrences(text, word);
    std::cout << "Кількість знайдених збігів: " << occurrences << "\n";

    
    std::cout << "\n-----------------------------------------\n";
    std::cout << "Спробуйте ввести власний текст:\n";

    std::string userText, userWord;
    std::cout << "Введіть текст: ";
    std::getline(std::cin, userText);

    std::cout << "Введіть слово для пошуку: ";
    std::cin >> userWord;

    int userCount = countWordOccurrences(userText, userWord);
    std::cout << "Слово \"" << userWord << "\" зустрічається разів: " << userCount << "\n";

    return 0;
}
11.10.26 практика
#include <iostream>
#include <string>


void parseEmail() {
    std::cout << "=== ЗАВДАННЯ 1: РОЗБІР EMAIL ===\n";
    std::cout << "Введіть email адресу: ";

    std::string email;
    std::getline(std::cin, email);


    size_t atPos = email.find('@');

 
    if (atPos == std::string::npos) {
        std::cout << "Помилка: Некоректний email (символ '@' не знайдено)!\n";
        return;
    }

    if (atPos == 0 || atPos == email.length() - 1) {
        std::cout << "Помилка: Некоректний email (відсутнє ім'я користувача або домен)!\n";
        return;
    }

    std::string username = email.substr(0, atPos);
    std::string domain = email.substr(atPos + 1);

    std::cout << "Ім'я користувача: " << username << "\n";
    std::cout << "Домен:            " << domain << "\n";
}


void textFilter() {
    std::cout << "\n=== ЗАВДАННЯ 2: ТЕКСТОВИЙ ФІЛЬТР ===\n";

    std::string sentence;
    std::string forbiddenWord;

    std::cout << "Введіть речення: ";
    std::getline(std::cin, sentence);

    std::cout << "Введіть заборонене слово: ";
    std::getline(std::cin, forbiddenWord);

    if (forbiddenWord.empty()) {
        std::cout << "Заборонене слово не вказано.\n";
        return;
    }

    size_t pos = sentence.find(forbiddenWord);

    if (pos != std::string::npos) {

        while (pos != std::string::npos) {
            sentence.replace(pos, forbiddenWord.length(), "****");

            pos = sentence.find(forbiddenWord, pos + 4);
        }

        sentence += " [Відредаговано]";

        std::cout << "\nРезультат фільтрації:\n" << sentence << "\n";
    }
    else {
        std::cout << "\nЗаборонених слів не знайдено. Початкове речення:\n" << sentence << "\n";
    }
}

int main() {
    parseEmail();
    textFilter();

    return 0;
}