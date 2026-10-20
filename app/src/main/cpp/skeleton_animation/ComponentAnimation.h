#ifndef CPP_SKELETON_ANIMATION_COMPONENT_ANIMATION_H
#define CPP_SKELETON_ANIMATION_COMPONENT_ANIMATION_H

#include <string>
#include <vector>
#include <map>
#include "Canvas.h"

class ComponentAnimation {
public:
    class MainComponent;
    class SonComponent {
    public:
        int father_nodeX;
        int father_nodeY;
        int son_nodeX;
        int son_nodeY;
        int trust_nodeX;
        int trust_nodeY;
        int w;
        int h;
        float angle;
        std::string bmp_path;
        std::vector<SonComponent*> l;
        int id;
        int father_id;
        std::vector<std::string> cmds;
        bool visible;
        Matrix* matrix;
        Bitmap* bmp;
        std::string vm_code;

        SonComponent(int father_nodeX, int father_nodeY, int son_nodeX, int son_nodeY, int w, int h, std::string bmp_path, int id, bool visible,std::string vm_code);

        virtual ~SonComponent() {}

        virtual SonComponent* copyClass();

        virtual void setAngle(int angle);
        virtual void addSon(SonComponent* son);
        virtual std::vector<SonComponent*> getSons();
        void compute_son(SonComponent* father);
        static float angle_swap(int dx, int dy);
        static std::vector<int> use_angle_compute_dxdy(float distant, float angle);
        static float toRadian(float angle);
        void draw(Canvas* ctx, MainComponent* mainComponent);
        std::vector<SonComponent*> getComponents(MainComponent* mainComponent);
    };

    class MainComponent : public SonComponent {
    public:
        std::string name;
        int centerX;
        int centerY;
        std::vector<SonComponent*> l;
        int id;
        bool visible;
        std::vector<SonComponent*> son_cache;
        bool horizontal_mirror;
        bool vertical_mirror;
        std::map<std::string, double> data;
        int index;
        int px;
        int py;

        MainComponent(int w, int h, std::string bmp_path, int id, bool visible);

        MainComponent* copyClass() override;

        virtual ~MainComponent() {}
        void setAngle(int angle) override;
        void addSon(SonComponent* son) override;
        std::vector<SonComponent*> getSons() override;
        void compute_trust();
        std::vector<SonComponent*> getComponents();
        void setHorizontalMirror(bool b);
        void setVerticalMirror(bool b);
    };

    static std::map<std::string, Bitmap*> map;
    static void render(MainComponent* mainComponent, Canvas* ctx);
    static Bitmap* getBitmapCache(std::string path);
    static void single(MainComponent* mainComponent, SonComponent* son);
    static void destroyAll();
};

#endif