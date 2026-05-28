#include <iostream>
#include <memory>
#include "gpu.h"
#include "container.h"
#include "sqlite_container.h"

using namespace std;

void ProcessAllGPUs(Iterator& it, const string& action_desc) {
    cout << "=== " << action_desc << " ===" << endl;
    int count = 0;
    for (it.First(); !it.Is_Done(); it.Next()) {
        GPU* current_gpu = it.Current_Item();
        if (current_gpu) {
            current_gpu->Render();
            count++;
        }
    }
    if (count == 0) cout << "Нет подходящих видеокарт" << endl;
    cout << endl;
}

int main() {
    cout << "=== Создание контейнера и добавление видеокарт ===" << endl;
    Container gpu_container;

    gpu_container.Add(make_unique<RTX_GPU>("NVIDIA RTX 4090", "GDDR6X", 24, 450, true));
    gpu_container.Add(make_unique<Integrated_GPU>("Intel UHD Graphics 770", "Shared", 2, 65, true));
    gpu_container.Add(make_unique<Pro_GPU>("NVIDIA RTX A6000", "GDDR6 ECC", 48, 300, true));

    Sorted_Container sorted_gpu_container;

    sorted_gpu_container.Add(make_unique<RTX_GPU>("NVIDIA RTX 4090", "GDDR6X", 24, 450, true));
    sorted_gpu_container.Add(make_unique<Integrated_GPU>("Intel UHD Graphics 770", "Shared", 2, 65, true));
    sorted_gpu_container.Add(make_unique<Pro_GPU>("NVIDIA RTX A6000", "GDDR6 ECC", 48, 300, true));

    cout << "=== Демонстрация работы Итератора ===" << endl;

    unique_ptr<Iterator> it = gpu_container.Create_Iterator();

    for (it->First(); !it->Is_Done(); it->Next()) {
        GPU* current_gpu = it->Current_Item();

        if (current_gpu) {
            current_gpu->Render();
        }
    }

    cout << "=== Демонстрация копирования контейнера ===" << endl;
    Container copied_container = gpu_container;

    unique_ptr<Iterator> copy_it = copied_container.Create_Iterator();
    copy_it->First();
    if (!copy_it->Is_Done()) {
        cout << "Первый элемент из скопированного контейнера:" << endl;
        copy_it->Current_Item()->Render();
    }

    cout << "\n=== Демонстрация: Итератор не зависит от реализации контейнера ===" << endl;
    auto it_sorted = sorted_gpu_container.Create_Iterator();
    ProcessAllGPUs(*it_sorted, "Обход ОТСОРТИРОВАННОГО контейнера");

    cout << "=== Демонстрация работы Декораторов ===" << endl;

    auto base_it1 = sorted_gpu_container.Create_Iterator();
    auto family_dec = make_unique<Family_Filter_Decorator<RTX_GPU>>(std::move(base_it1));
    ProcessAllGPUs(*family_dec, "Декоратор 1: Только видеокарты RTX_GPU");

    auto base_it2 = sorted_gpu_container.Create_Iterator();
    auto vram_dec = make_unique<VRAM_Filter_Decorator>(std::move(base_it2), 24);
    ProcessAllGPUs(*vram_dec, "Декоратор 2: Только видеокарты с VRAM >= 24 ГБ");

    auto base_it3 = sorted_gpu_container.Create_Iterator();
    auto combo_family = make_unique<Family_Filter_Decorator<RTX_GPU>>(std::move(base_it3));
    auto combo_vram = make_unique<VRAM_Filter_Decorator>(std::move(combo_family), 24);
    ProcessAllGPUs(*combo_vram, "Декораторы 1 и 2: RTX_GPU И VRAM >= 24 ГБ");

    // Этот блок кода тестирует работу нового SQLite контейнера
    cout << "\n=== Демонстрация работы SQLite Контейнера ===" << endl;
    SQLite_Container db_container("gpus_test.db");

    db_container.Add(make_unique<RTX_GPU>("NVIDIA RTX 5090", "GDDR7", 32, 500, true));
    db_container.Add(make_unique<Pro_GPU>("AMD Radeon Pro W7900", "GDDR6 ECC", 48, 295, true));

    auto db_it = db_container.Create_Iterator();
    ProcessAllGPUs(*db_it, "Обход видеокарт, сохраненных в базе данных SQLite");

    return 0;
}