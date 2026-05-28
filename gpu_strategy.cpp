#include "gpu_strategy.h"
#include <iostream>

// Стратегия 1: делегирует вызов методу Render()
void Render_Strategy::Execute(GPU* gpu) const {
    if (gpu) gpu->Render();
}

// Стратегия 2: делегирует вызов методу Generate_Ai_Picture()
void AI_Generate_Strategy::Execute(GPU* gpu) const {
    if (gpu) gpu->Generate_Ai_Picture();
}

// Стратегия 3: делегирует вызов методу Mining()
void Mining_Strategy::Execute(GPU* gpu) const {
    if (gpu) gpu->Mining();
}


// Конструктор принимает начальную стратегию через unique_ptr
GPU_Context::GPU_Context(std::unique_ptr<IStrategy> s)
    : strategy(std::move(s)) {}

// Замена стратегии во время выполнения
void GPU_Context::Set_Strategy(std::unique_ptr<IStrategy> s) {
    strategy = std::move(s);
}

// Выполнить текущую стратегию над указанным GPU
void GPU_Context::Run(GPU* gpu) const {
    if (strategy && gpu) {
        strategy->Execute(gpu);
    }
}
