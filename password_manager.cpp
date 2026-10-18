
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <windows.h>

using namespace std;

struct PasswordEntry {
    string site;
    string login;
    string password;
};

struct Folder {
    string name;
    vector<PasswordEntry> entries;
};

vector<Folder> folders;

const string DATA_FILE = "passwords.txt";

void pauseProgram() {
    cout << "\nНажми Enter...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void saveData() {
    ofstream file(DATA_FILE);

    if (!file.is_open()) {
        cout << "Ошибка сохранения файла!\n";
        return;
    }

    file << folders.size() << "\n";

    for (const auto& folder : folders) {
        file << folder.name << "\n";
        file << folder.entries.size() << "\n";

        for (const auto& entry : folder.entries) {
            file << entry.site << "\n";
            file << entry.login << "\n";
            file << entry.password << "\n";
        }
    }

    file.close();
}

void loadData() {
    ifstream file(DATA_FILE);

    if (!file.is_open()) {
        return;
    }

    size_t folderCount;
    file >> folderCount;
    file.ignore();

    for (size_t i = 0; i < folderCount; i++) {
        Folder folder;

        getline(file, folder.name);

        size_t entryCount;
        file >> entryCount;
        file.ignore();

        for (size_t j = 0; j < entryCount; j++) {
            PasswordEntry entry;

            getline(file, entry.site);
            getline(file, entry.login);
            getline(file, entry.password);

            folder.entries.push_back(entry);
        }

        folders.push_back(folder);
    }

    file.close();
}

void createFolder() {
    system("cls");

    cout << "=== СОЗДАТЬ ПАПКУ ===\n\n";

    string name;

    cout << "Название папки: ";
    cin.ignore();
    getline(cin, name);

    if (name.empty()) {
        cout << "\nНазвание не может быть пустым.\n";
        pauseProgram();
        return;
    }

    folders.push_back({name, {}});

    saveData();

    cout << "\nПапка создана!\n";
    pauseProgram();
}

void deleteFolder() {
    system("cls");

    if (folders.empty()) {
        cout << "Папок нет.\n";
        pauseProgram();
        return;
    }

    cout << "=== УДАЛЕНИЕ ПАПКИ ===\n\n";

    for (int i = 0; i < folders.size(); i++) {
        cout << i + 1 << ". "
             << folders[i].name
             << " | Записей: "
             << folders[i].entries.size()
             << "\n";
    }

    cout << "\nНомер папки: ";

    int choice;
    cin >> choice;

    if (choice < 1 || choice > folders.size()) {
        cout << "\nНеверный номер.\n";
        pauseProgram();
        return;
    }

    cout << "\nУдалить папку \""
         << folders[choice - 1].name
         << "\" и все пароли внутри?\n";

    cout << "1. Да\n";
    cout << "2. Нет\n";
    cout << "Выбор: ";

    int confirm;
    cin >> confirm;

    if (confirm == 1) {
        folders.erase(folders.begin() + choice - 1);
        saveData();

        cout << "\nПапка удалена.\n";
    } else {
        cout << "\nУдаление отменено.\n";
    }

    pauseProgram();
}

void addPassword(Folder& folder) {
    system("cls");

    cout << "=== НОВАЯ ЗАПИСЬ ===\n\n";

    PasswordEntry entry;

    cin.ignore();

    cout << "Сайт/программа: ";
    getline(cin, entry.site);

    cout << "Логин: ";
    getline(cin, entry.login);

    cout << "Пароль: ";
    getline(cin, entry.password);

    folder.entries.push_back(entry);

    saveData();

    cout << "\nЗапись сохранена!\n";
    pauseProgram();
}

void showPasswords(Folder& folder) {
    system("cls");

    cout << "=== ПАПКА: "
         << folder.name
         << " ===\n\n";

    if (folder.entries.empty()) {
        cout << "Здесь пока нет записей.\n";
        pauseProgram();
        return;
    }

    for (int i = 0; i < folder.entries.size(); i++) {
        cout << "-----------------------------\n";

        cout << "Запись #" << i + 1 << "\n";
        cout << "Сайт: " << folder.entries[i].site << "\n";
        cout << "Логин: " << folder.entries[i].login << "\n";
        cout << "Пароль: " << folder.entries[i].password << "\n";
    }

    cout << "-----------------------------\n";

    pauseProgram();
}

void deletePassword(Folder& folder) {
    system("cls");

    if (folder.entries.empty()) {
        cout << "Нет записей для удаления.\n";
        pauseProgram();
        return;
    }

    cout << "=== УДАЛЕНИЕ ЗАПИСИ ===\n\n";

    for (int i = 0; i < folder.entries.size(); i++) {
        cout << i + 1
             << ". "
             << folder.entries[i].site
             << " | "
             << folder.entries[i].login
             << "\n";
    }

    cout << "\nНомер записи: ";

    int choice;
    cin >> choice;

    if (choice < 1 || choice > folder.entries.size()) {
        cout << "\nНеверный номер.\n";
        pauseProgram();
        return;
    }

    folder.entries.erase(
        folder.entries.begin() + choice - 1
    );

    saveData();

    cout << "\nЗапись удалена.\n";
    pauseProgram();
}

void openFolder(int index) {
    bool running = true;

    while (running) {
        system("cls");

        Folder& folder = folders[index];

        cout << "==============================\n";
        cout << "ПАПКА: " << folder.name << "\n";
        cout << "==============================\n\n";

        cout << "1. Показать пароли\n";
        cout << "2. Добавить пароль\n";
        cout << "3. Удалить пароль\n";
        cout << "4. Назад\n";

        cout << "\nВыбор: ";

        int choice;
        cin >> choice;

        if (choice == 1) {
            showPasswords(folder);
        }
        else if (choice == 2) {
            addPassword(folder);
        }
        else if (choice == 3) {
            deletePassword(folder);
        }
        else if (choice == 4) {
            running = false;
        }
    }
}

void showFolders() {
    system("cls");

    if (folders.empty()) {
        cout << "Папок пока нет.\n";
        pauseProgram();
        return;
    }

    cout << "=== МОИ ПАПКИ ===\n\n";

    for (int i = 0; i < folders.size(); i++) {
        cout << i + 1
             << ". "
             << folders[i].name
             << " | Записей: "
             << folders[i].entries.size()
             << "\n";
    }

    cout << "\nВведите номер папки\n";
    cout << "0 — назад\n\n";
    cout << "Выбор: ";

    int choice;
    cin >> choice;

    if (choice == 0) {
        return;
    }

    if (choice >= 1 && choice <= folders.size()) {
        openFolder(choice - 1);
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    loadData();

    bool running = true;

    while (running) {
        system("cls");

        cout << "====================================\n";
        cout << "       PASSWORD MANAGER\n";
        cout << "====================================\n\n";

        cout << "Папок: " << folders.size() << "\n\n";

        cout << "1. Открыть папку\n";
        cout << "2. Создать папку\n";
        cout << "3. Удалить папку\n";
        cout << "4. Выйти\n";

        cout << "\nВыбор: ";

        int choice;
        cin >> choice;

        if (choice == 1) {
            showFolders();
        }
        else if (choice == 2) {
            createFolder();
        }
        else if (choice == 3) {
            deleteFolder();
        }
        else if (choice == 4) {
            saveData();
            running = false;
        }
    }

    system("cls");

    cout << "Данные сохранены.\n";
    cout << "Менеджер закрыт.\n";

    return 0;
}