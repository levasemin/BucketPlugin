#pragma once

#include "PluginStandart.hpp"
#include <string>
#include <stack>
#include <iostream>

class ToolBucket : public booba::Tool
{
public:
    class Vector2d
    {
    public:
        int x_;
        int y_;

        Vector2d(int x, int y):
            x_(x),
            y_(y)
        {}
    };

    ToolBucket();

    ToolBucket(const ToolBucket &) = default;
    ToolBucket &operator=(const ToolBucket &) = default;
    ~ToolBucket();
    
    void apply(booba::Image* image, booba::Image *hidden_layer, const booba::Event* event) override;

    const char* getTexture() override;

    std::pair<int, int> getShape() override;

private:
    std::string icon_path_ = "../PluginIcons/Bucket.png";

    int dispersion_ = 0;
    booba::Color current_color_;
    
    booba::WidgetCreator *widget_creator_ = nullptr;
    booba::Editor *dispersion_editor_ = nullptr;
    
    void setDispersion(std::string string);
    void fill_part(booba::Image *image, ToolBucket::Vector2d position);
    std::pair<ToolBucket::Vector2d, ToolBucket::Vector2d> fill_ray(booba::Image *image, ToolBucket::Vector2d position);

};