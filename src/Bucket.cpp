#include "Bucket.hpp"

bool color_eq(const booba::Color &color1, const booba::Color &color2, int dispersion)
{
    return abs(int(color1.r) - int(color2.r)) <= dispersion &&
           abs(int(color1.g) - int(color2.g)) <= dispersion &&
           abs(int(color1.b) - int(color2.b)) <= dispersion &&
           abs(int(color1.a) - int(color2.a)) <= dispersion;
}
ToolBucket::ToolBucket() : booba::Tool()
{
    widget_creator_ = (booba::WidgetCreator *)booba::getWidgetCreator(this);
    
    dispersion_editor_ = (booba::Editor *)    widget_creator_->createWidget(booba::WidgetCreator::Type::EDITOR,    50,  30, 50, 100);

    dispersion_editor_->setCommand((booba::Command<std::string> *) new booba::SimpleCommand<ToolBucket, std::string>(this, &ToolBucket::setDispersion));
}

ToolBucket::~ToolBucket()
{
    delete dispersion_editor_->getCommand();
}

void ToolBucket::setDispersion(std::string string)
{    
    if (string.size() == 0)
    {
        dispersion_ = 0;
        return;
    }

    if ('0' > string[string.size() - 1] || string[string.size() - 1] > '9')
    {
        string.resize(string.size() - 1);
        dispersion_editor_->setText(string);
        
        return;
    }
    
    dispersion_ = int(std::stoi(string));
}

std::pair<ToolBucket::Vector2d, ToolBucket::Vector2d> ToolBucket::fill_ray(booba::Image *image, ToolBucket::Vector2d position)
{
    booba::Color current_color = current_color_;

    ToolBucket::Vector2d left_pos = ToolBucket::Vector2d(0.f, position.y_);
    ToolBucket::Vector2d right_pos = ToolBucket::Vector2d(float(image->getW()) - 1, position.y_);

    for (int x = int(position.x_); x > -1; x--)
    {
        booba::Color im_color = image->getPixel(x, int(position.y_));
        booba::Color image_color = im_color;

        if (color_eq(image_color, current_color, dispersion_))
        {
            image->setPixel(x, int(position.y_), booba::APPCONTEXT->fgColor);
        }

        else
        {
            left_pos.x_ = float(x + 1);
            break;
        }
    } 

    for (int x = int(position.x_) + 1; x < int(image->getW()); x ++)
    {
        booba::Color im_color = image->getPixel(x, int(position.y_));
        booba::Color image_color = im_color;

        if (color_eq(image_color, current_color, dispersion_))
        {
            image->setPixel(x, int(position.y_), booba::APPCONTEXT->fgColor);
        }

        else
        {
            right_pos.x_ = float(x - 1);
            break;
        }
    } 
    
    return {left_pos, right_pos};
}

void ToolBucket::fill_part(booba::Image *image, ToolBucket::Vector2d position)
{
    booba::Color current_color = current_color_;

    std::stack<ToolBucket::Vector2d> zatr_pixels;
    zatr_pixels.push(position);

    while (!zatr_pixels.empty())
    {        
        position = zatr_pixels.top();
        zatr_pixels.pop();

        std::pair<ToolBucket::Vector2d, ToolBucket::Vector2d> result = fill_ray(image, position);
        ToolBucket::Vector2d left_position = result.first;
        ToolBucket::Vector2d right_position = result.second;

        for (int x = int(left_position.x_); x <= int(right_position.x_); x++)
        {
            booba::Color im_color = image->getPixel(x, int(position.y_) + 1);
            booba::Color pixel_color = im_color;

            if (color_eq(pixel_color, current_color, dispersion_) && 
               !color_eq(pixel_color, booba::APPCONTEXT->fgColor, 0))    
            {
                for (; color_eq(pixel_color, current_color, dispersion_) &&
                      !color_eq(pixel_color, booba::APPCONTEXT->fgColor, 0) && 
                       x < int(image->getW()); x++)
                {
                    pixel_color = image->getPixel(x, int(position.y_ + 1));
                }
                
                if (uint32_t(position.y_) + 1 < image->getH())
                {
                    zatr_pixels.push(ToolBucket::Vector2d(float(x - 2), position.y_ + 1));
                }
            }
        }

        for (int x = int(left_position.x_); x <= int(right_position.x_); x++)
        {
            booba::Color im_color = image->getPixel(x, int(position.y_) - 1);
            booba::Color pixel_color = im_color;

            if (color_eq(pixel_color, current_color, dispersion_) &&
               !color_eq(pixel_color, booba::APPCONTEXT->fgColor, 0))     
            {
                for (; color_eq(pixel_color, current_color, dispersion_) &&
                      !color_eq(pixel_color, booba::APPCONTEXT->fgColor, 0) &&
                       x < int(image->getW()); x++)
                {
                    pixel_color = image->getPixel(x, int(position.y_ - 1));
                }
                
                if (position.y_ > 1)
                {
                    zatr_pixels.push(ToolBucket::Vector2d(float(x - 2), position.y_ - 1));
                }
            }
        }
    }
}

void ToolBucket::apply(booba::Image* image, booba::Image *hidden_layer, const booba::Event* event)
{        
    switch (event->type)
    {
        case booba::Event::MousePressed:
        {
            ToolBucket::Vector2d position((float)event->Oleg.mpedata.x, (float)event->Oleg.mpedata.y);
            current_color_ = image->getPixel(int(position.x_), int(position.y_));

            fill_part(image, position);

            break;
        }
        case booba::Event::MouseReleased:
        case booba::Event::MouseMoved:
        case booba::Event::NoEvent:
        
        default:
            break;
    }
}

const char* ToolBucket::getTexture()
{
    return icon_path_.c_str();
} 

std::pair<int, int> ToolBucket::getShape()
{
    return {200, 200};
}

extern "C" void *init_module()
{
    return new ToolBucket();
}