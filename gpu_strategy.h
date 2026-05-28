#ifndef LABA5_2_GPU_STRATEGY_H
#define LABA5_2_GPU_STRATEGY_H
#pragma once

#include "gpu.h"
#include <memory>


// Абстрактный интерфейс стратегии взаимодействия с GPU
class IStrategy {
public:
    virtual ~IStrategy() = default;

    // Выполнить действие над переданным объектом GPU
    virtual void Execute(GPU* gpu) const = 0;
};

// Стратегия 1: Рендеринг - вызывает Render()
class Render_Strategy : public IStrategy {
public:
    void  Execute(GPU* gpu) const override;
};

// Стратегия 2: Генерация ИИ-изображения - вызывает Generate_Ai_Picture()
class AI_Generate_Strategy : public IStrategy {
public:
    void Execute(GPU* gpu) const override;
};

// Стратегия 3: Майнинг - вызывает Mining()
class Mining_Strategy : public IStrategy {
public:
    void Execute(GPU* gpu) const override;
};


class GPU_Context {
private:
    // Умный указатель на текущую стратегию
    std::unique_ptr<IStrategy> strategy;

public:
    // Установить начальную стратегию через конструктор
    explicit GPU_Context(std::unique_ptr<IStrategy> s);

    // Заменить стратегию во время выполнения
    void Set_Strategy(std::unique_ptr<IStrategy> s);

    // Выполнить текущую стратегию над GPU
    void Run(GPU* gpu) const;
};

#endif //LABA5_2_GPU_STRATEGY_H
