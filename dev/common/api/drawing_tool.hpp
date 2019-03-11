#ifndef DRAWING_TOOL_HPP_
#define DRAWING_TOOL_HPP_

template<class T>
class IDrawingTool {
    public:
        virtual void Draw(const T& type) = 0;
        virtual void Flush() = 0;
};

#endif // DRAWING_TOOL_HPP_

