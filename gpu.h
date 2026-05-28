#ifndef LABA5_2_GPU_H
#define LABA5_2_GPU_H
#pragma once

#include <iostream>
#include <string>
#include <map>
#include <memory>

enum class GPU_Type {
    RTX,
    INTEGRATED,
    PRO
};

extern const std::map<GPU_Type, std::string> GPU_Type_Names;

class GPU {
protected:
    std::string model;
    std::string type_vramGB;
    int vramGB;
    int tdpW;

public:
    GPU(std::string name, std::string type_vram, int vram, int tdp);

    GPU(const GPU& other);

    virtual ~GPU() {};

    int Get_VRAM() const;

    virtual bool operator==(const GPU& other) const;
    virtual bool operator<(const GPU& other) const;
    virtual bool operator>(const GPU& other) const;

    virtual void Render();
    virtual void Generate_Ai_Picture();
    virtual void Mining();

    virtual std::unique_ptr<GPU> Copy() const;

    std::string Get_Model() const { return model; }
    std::string Get_Type_VRAM() const { return type_vramGB; } //Новые методы в базовый класс GPU для того, чтобы класс SQLite мог прочитать значения
    int Get_TDP() const { return tdpW; }

    virtual GPU_Type Get_Type_Enum() const {return GPU_Type::PRO;}
    virtual int Get_Extra_Param() const {return 0;}
};

class RTX_GPU : public GPU {
private:
    bool rt_core;

public:
    RTX_GPU(std::string name, std::string type_vram, int vram, int tdp, bool rt);

    RTX_GPU(const RTX_GPU& other);

    void Ray_Tracing();
    void Render() override;
    void Generate_Ai_Picture() override;
    void Mining() override;
    GPU_Type Get_Type_Enum() const override { return GPU_Type::RTX; }
    int Get_Extra_Param() const override { return rt_core; }

    std::unique_ptr<GPU> Copy() const override;
};

class Integrated_GPU : public GPU {
private:
    bool uses_shared_ram;

public:
    Integrated_GPU(std::string name, std::string type_vram, int vram, int tdp, bool shared_ram);

    Integrated_GPU(const Integrated_GPU& other);

    void Render() override;
    void Generate_Ai_Picture() override;
    void Mining() override;
    GPU_Type Get_Type_Enum() const override { return GPU_Type::INTEGRATED; }
    int Get_Extra_Param() const override { return uses_shared_ram; }

    std::unique_ptr<GPU> Copy() const override;
};

class Pro_GPU : public GPU {
private:
    bool has_ecc_memory;

public:
    Pro_GPU(std::string name, std::string type_vram, int vram, int tdp, bool ecc);

    Pro_GPU(const Pro_GPU& other);

    void Render() override;
    void Generate_Ai_Picture() override;
    void Mining() override;
    GPU_Type Get_Type_Enum() const override { return GPU_Type::PRO; }
    int Get_Extra_Param() const override { return has_ecc_memory; }

    std::unique_ptr<GPU> Copy() const override;
};

class GPU_Wrapper {
public:
    std::shared_ptr<GPU> ptr;

    GPU_Wrapper() = default;
    GPU_Wrapper(std::shared_ptr<GPU> p);

    bool operator==(const GPU_Wrapper& other) const;
    bool operator<(const GPU_Wrapper& other) const;
    bool operator>(const GPU_Wrapper& other) const;
};

#endif //LABA5_2_GPU_H
