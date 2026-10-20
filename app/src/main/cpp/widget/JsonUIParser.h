#ifndef QIANLAN_JSON_UI_PARSER_H
#define QIANLAN_JSON_UI_PARSER_H

#include <string>
#include <vector>
#include <map>

// 前向声明，避免头文件相互包含
class Canvas;
class Paint;
class Bitmap;
class RectF;
class UnitDataCommand;

namespace JsonUIParserNS { }

class JsonUIParser {
public:
    // ==================== UIElement 基类 ====================
    class UIElement {
    public:
        std::string id;
        float x = 0.5f, y = 0.5f;          // 相对坐标 (0-1)
        float width = 0.3f, height = 0.1f; // 相对宽高 (0-1)

        float fixedX = 0.f, fixedY = 0.f;
        float fixedWidth = 0.f, fixedHeight = 0.f;
        bool hasFixedX = false, hasFixedY = false;
        bool hasFixedWidth = false, hasFixedHeight = false;

        RectF* rect = nullptr;
        bool visible = true;
        bool centerAnchor = false;         // true: x,y 为中心点; false: 左上角

        std::string vm_code;

        UIElement(const std::string& id, float x, float y,
                  float width, float height,
                  const float* fixedX, const float* fixedY,
                  const float* fixedWidth, const float* fixedHeight,
                  bool visible,
                  bool centerAnchor,std::string vm_code);

        virtual ~UIElement();

        virtual void updateRect(int viewWidth, int viewHeight);

        virtual void draw(Canvas* canvas, int viewWidth, int viewHeight, bool visible) = 0;

        virtual bool contains(float touchX, float touchY);

        virtual void setPressed(bool pressed);
        virtual void setPressed(bool pressed, float x, float y);

        std::string getId() const { return id; }

        bool getVisible() const { return visible; }
        void setVisible(bool v) { visible = v; }
    };

    // ==================== UITextView ====================
    class UITextView : public UIElement {
    public:
        std::string text;
        int textColor = 0xFFFFFFFF;
        float textSize = 40.f;
        int align = 0;              // 0=LEFT, 1=RIGHT, 2=CENTER (与 Paint::Align 对应)
        Paint* paint = nullptr;
        float maxWidth = 1.f;
        std::string fontPath;       // ★ 新增：自定义字体路径

        UITextView(const std::string& id, float x, float y,
                   float width, float height,
                   const float* fixedX, const float* fixedY,
                   const float* fixedWidth, const float* fixedHeight,
                   const std::string& text, int textColor, float textSize,
                   int gravity, const std::string& fontPath,
                   bool visible,
                   float maxWidth, bool centerAnchor,std::string vm_code);

        ~UITextView() override;

        void draw(Canvas* canvas, int viewWidth, int viewHeight, bool visible) override;
    };

    // ==================== UIButton ====================
    class UIButton : public UIElement {
    public:
        std::string text;
        int normalColor = 0xFF6495ED;
        int pressedColor = 0xFF4682B4;
        int textColor = 0xFFFFFFFF;
        float textSize = 60.f;
        Bitmap* backgroundImage = nullptr;
        Paint* paint = nullptr;
        Paint* textPaint = nullptr;
        bool isPressed = false;

        UIButton(const std::string& id, float x, float y,
                 float width, float height,
                 const float* fixedX, const float* fixedY,
                 const float* fixedWidth, const float* fixedHeight,
                 const std::string& text, int normalColor, int pressedColor,
                 int textColor, float textSize, const std::string& bgImagePath,
                 bool visible,
                 bool centerAnchor,std::string vm_code);

        ~UIButton() override;

        void setBackgroundImage(Bitmap* bitmap);

        void setPressed(bool pressed) override;
        void setPressed(bool pressed, float x, float y) override;  // ★ 新增

        void draw(Canvas* canvas, int viewWidth, int viewHeight, bool visible) override;
    };

    // ==================== UIImage ====================
    class UIImage : public UIElement {
    public:
        Bitmap* bitmap = nullptr;
        Paint* paint = nullptr;

        UIImage(const std::string& id, float x, float y,
                float width, float height,
                const float* fixedX, const float* fixedY,
                const float* fixedWidth, const float* fixedHeight,
                Bitmap* bitmap, bool visible,
                bool centerAnchor,std::string vm_code);

        ~UIImage() override;

        void draw(Canvas* canvas, int viewWidth, int viewHeight, bool visible) override;
    };

    // ==================== UIRectangle ====================
    class UIRectangle : public UIElement {
    public:
        int fillColor = 0xFF808080;
        int strokeColor = 0xFF000000;
        float strokeWidth = 0.f;
        float cornerRadius = 0.f;
        Paint* fillPaint = nullptr;
        Paint* strokePaint = nullptr;

        UIRectangle(const std::string& id, float x, float y,
                    float width, float height,
                    const float* fixedX, const float* fixedY,
                    const float* fixedWidth, const float* fixedHeight,
                    int fillColor, int strokeColor, float strokeWidth,
                    float cornerRadius, bool visible,
                    bool centerAnchor,std::string vm_code);

        ~UIRectangle() override;

        void draw(Canvas* canvas, int viewWidth, int viewHeight, bool visible) override;
    };

    // ==================== UIGrid ====================
    class UIGrid : public UIElement {
    public:
        int rows = 4;
        int columns = 4;
        float cellSpacing = 5.f;
        int cellBgColor = 0xFF333333;
        int cellBorderColor = 0xFF666666;
        float cellBorderWidth = 1.f;
        float cornerRadius = 10.f;
        int gridBgColor = 0xCC000000;

        Paint* bgPaint = nullptr;
        Paint* cellBgPaint = nullptr;
        Paint* cellBorderPaint = nullptr;
        Paint* gridBitmapPaint = nullptr;   // ★ 新增：绘制格子图标专用

        std::string listName;
        int pressedRow = -1;
        int pressedCol = -1;
        int pressedCellBgColor = 0xFF000000;

        std::vector<std::string> list_bmp_str;
        std::vector<Bitmap*> list_bmp;

        UIGrid(const std::string& id, float x, float y,
               float width, float height,
               const float* fixedX, const float* fixedY,
               const float* fixedWidth, const float* fixedHeight,
               int rows, int columns, float cellSpacing,
               int cellBgColor, int cellBorderColor, float cellBorderWidth,
               float cornerRadius, int gridBgColor,
               bool visible,
               bool centerAnchor, const std::string& listName,
               int pressedCellBgColor,std::string vm_code);

        ~UIGrid() override;

        std::vector<int> getCellAt(float touchX, float touchY);
        void reload(int i, const std::string& path);

        void setPressed(bool pressed) override;
        void setPressed(bool pressed, float x, float y) override;

        void draw(Canvas* canvas, int viewWidth, int viewHeight, bool visible) override;
    };

    // ==================== JsonUIParser 本体 ====================
public:
    JsonUIParser();
    ~JsonUIParser();

    void addAll(const std::vector<UIElement*>& l);
    std::vector<UIElement*>& addElement(UIElement* newElement);
    void remove(const std::string& id);

    void setXY(const std::string& id, float x, float y);
    void setWH(const std::string& id, float w, float h);
    void setId(const std::string& id, const std::string& newId);

    bool getVisible(const std::string& id);
    void setVisible(const std::string& id, bool b);

    std::vector<UIElement*>& loadFromAssets(const std::string& filePath);
    void clearUi();

    // 静态点击分发（对应 Java 的 click_listener）
    static bool clickListener(int action, float x, float y);

    std::vector<UIElement*>& parse(const std::string& json);

    UIButton* getButtonById(const std::string& id);
    std::vector<UIElement*>& getElements();

    void draw(Canvas* canvas, int viewWidth, int viewHeight);

    static RectF calculateRect(float x, float y, float width, float height,
                               int viewWidth, int viewHeight);

    static int parseColor(const std::string& colorStr);
    static int parseGravity(const std::string& gravity);
    static Bitmap* loadBitmap(const std::string& assetPath);

private:
    static std::vector<UIElement*> elements;
    std::map<std::string, UIButton*> buttonMap;

    static UIElement* pressedButton;
};

#endif // QIANLAN_JSON_UI_PARSER_H