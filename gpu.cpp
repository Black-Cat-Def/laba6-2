#include "gpu.h"

using namespace std;

const map<GPU_Type, string> GPU_Type_Names = {
    {GPU_Type::RTX, "Игровая видеокарта"},
    {GPU_Type::INTEGRATED, "Встроенная графика"},
    {GPU_Type::PRO, "Профессиональная видеокарта"}
};

int GPU::Get_VRAM() const {
    return vramGB;
}

GPU::GPU(string name, string type_vram, int vram, int tdp)
    : model(name), type_vramGB(type_vram), vramGB(vram), tdpW(tdp) {}

GPU::GPU(const GPU& other)
    : model(other.model), type_vramGB(other.type_vramGB), vramGB(other.vramGB), tdpW(other.tdpW) {}

bool GPU::operator==(const GPU& other) const {
    return vramGB == other.vramGB && tdpW == other.tdpW && model == other.model;
}

bool GPU::operator<(const GPU& other) const {
    if (vramGB != other.vramGB) return vramGB < other.vramGB;
    if (tdpW != other.tdpW) return tdpW < other.tdpW;
    return model < other.model;
}

bool GPU::operator>(const GPU& other) const {
    return other < *this;
}

void GPU::Render() {
    cout << model << ": Выводит изображение" << endl;
}

void GPU::Generate_Ai_Picture() {
    cout << model << ": Выполняется генерация при помощи ИИ" << endl;
}

void GPU::Mining() {
    cout << model << ": Выполняется майнинг криптовалюты" << endl;
}

unique_ptr<GPU> GPU::Copy() const {
    return make_unique<GPU>(*this);
}

RTX_GPU::RTX_GPU(string name, string type_vram, int vram, int tdp, bool rt)
    : GPU(name, type_vram, vram, tdp), rt_core(rt) {}

RTX_GPU::RTX_GPU(const RTX_GPU& other)
    : GPU(other), rt_core(other.rt_core) {}

void RTX_GPU::Ray_Tracing() {
    cout << model << ": Выполняется просчёт лучей света в сцене" << endl;
}

void RTX_GPU::Render() {
    cout << model << ": Выводит 4К изображение с высокой частотой кадров" << endl;
}

void RTX_GPU::Generate_Ai_Picture() {
    cout << model << ": Выполняется генерация при помощи ИИ быстрее" << endl;
}

void RTX_GPU::Mining() {
    cout << model << ": Выполняется майнинг криптовалюты с высокой эффективностью" << endl;
}

unique_ptr<GPU> RTX_GPU::Copy() const {
    return make_unique<RTX_GPU>(*this);
}

Integrated_GPU::Integrated_GPU(string name, string type_vram, int vram, int tdp, bool shared_ram)
    : GPU(name, type_vram, vram, tdp), uses_shared_ram(shared_ram) {}

Integrated_GPU::Integrated_GPU(const Integrated_GPU& other)
    : GPU(other), uses_shared_ram(other.uses_shared_ram) {}

void Integrated_GPU::Render() {
    cout << model << ": Отрисовывает рабочий стол" << endl;
}

void Integrated_GPU::Generate_Ai_Picture() {
    cout << model << ": Не может сгенерировать ИИ картинку, не хватает мощности и памяти" << endl;
}

void Integrated_GPU::Mining() {
    cout << model << ": Майнинг невозможен, устройство слишком слабое" << endl;
}

unique_ptr<GPU> Integrated_GPU::Copy() const {
    return make_unique<Integrated_GPU>(*this);
}

Pro_GPU::Pro_GPU(string name, string type_vram, int vram, int tdp, bool ecc)
    : GPU(name, type_vram, vram, tdp), has_ecc_memory(ecc) {}

Pro_GPU::Pro_GPU(const Pro_GPU& other)
    : GPU(other), has_ecc_memory(other.has_ecc_memory) {}

void Pro_GPU::Render() {
    cout << model << ": Выполняется сверхточный рендеринг сложных 3D моделей" << endl;
}

void Pro_GPU::Generate_Ai_Picture() {
    cout << model << ": Обучает сложную нейросеть" << endl;
}

void Pro_GPU::Mining() {
    cout << model << ": Выполняется майнинг криптовалюты с максимальной эффективностью" << endl;
}

unique_ptr<GPU> Pro_GPU::Copy() const {
    return make_unique<Pro_GPU>(*this);
}

// Шаблонный метод: задаёт фиксированный порядок шагов
void GPU::Run_Benchmark() {
    Prepare_Benchmark();   // Шаг 1: подготовка
    Execute_Benchmark();   // Шаг 2: нагрузочный тест
    Collect_Results();     // Шаг 3: сбор результатов
    Print_Report();        // Шаг 4: итоговый отчёт (общий)
}

// Шаг 1 по умолчанию - общая подготовка для всех GPU
void GPU::Prepare_Benchmark() {
    cout << "[Подготовка] " << model << ": сброс драйверов и очистка кэша GPU" << endl;
}

// Шаг 2 по умолчанию - базовая заглушка
void GPU::Execute_Benchmark() {
    cout << "[Тест] " << model << ": выполнение стандартного нагрузочного теста" << endl;
}

// Шаг 3 по умолчанию - базовая заглушка
void GPU::Collect_Results() {
    cout << "[Результат] " << model << ": результаты получены" << endl;
}

// Шаг 4 — итоговый отчёт, одинаков для всех
void GPU::Print_Report() {
    cout << "[Отчёт] " << model
         << " | VRAM: " << vramGB << " ГБ"
         << " | TDP: " << tdpW << " Вт"
         << " | Тип: " << GPU_Type_Names.at(Get_Type_Enum())
         << endl;
}


// Шаг 2: RTX запускает тест на FPS в 3D сцене с трассировкой лучей
void RTX_GPU::Execute_Benchmark() {
    cout << "[Тест] " << model << ": запуск 3D сцены с трассировкой лучей (RTX On)" << endl;
}

// Шаг 3: RTX выводит результат в виде FPS
void RTX_GPU::Collect_Results() {
    int fps = vramGB * 10 + tdpW / 10; // условная формула для демонстрации
    cout << "[Результат] " << model << ": средний FPS = " << fps << endl;
}


// Шаг 1: Integrated дополнительно резервирует системную оперативную память
void Integrated_GPU::Prepare_Benchmark() {
    GPU::Prepare_Benchmark(); // вызываем общую подготовку базового класса
    cout << "[Подготовка] " << model << ": резервирование " << vramGB << " ГБ системной RAM для GPU" << endl;
}

// Шаг 2: Integrated запускает лёгкий 2D тест
void Integrated_GPU::Execute_Benchmark() {
    cout << "[Тест] " << model << ": запуск лёгкого 2D теста (3D недоступен)" << endl;
}

// Шаг 3: Integrated выводит синтетический балл
void Integrated_GPU::Collect_Results() {
    int score = vramGB * 50 + tdpW * 2; // условная формула для демонстрации
    cout << "[Результат] " << model << ": синтетический балл = " << score << endl;
}


// Шаг 2: Pro запускает тест на точность вычислений двойной точности
void Pro_GPU::Execute_Benchmark() {
    cout << "[Тест] " << model << ": запуск теста вычислений FP64 (двойная точность)" << endl;
}

// Шаг 3: Pro выводит результат в TFLOPS
void Pro_GPU::Collect_Results() {
    double tflops = vramGB * 0.5 + tdpW * 0.02; // условная формула для демонстрации
    cout << "[Результат] " << model << ": производительность FP64 = " << tflops << " TFLOPS" << endl;
}

GPU_Wrapper::GPU_Wrapper(std::shared_ptr<GPU> p) : ptr(p) {}

bool GPU_Wrapper::operator==(const GPU_Wrapper& other) const {
    return *ptr == *other.ptr;
}

bool GPU_Wrapper::operator<(const GPU_Wrapper& other) const {
    return *ptr < *other.ptr;
}

bool GPU_Wrapper::operator>(const GPU_Wrapper& other) const {
    return *ptr > *other.ptr;
}
