#ifndef _RENDER_H_
#define _RENDER_H_

class Render {
    public:
        void output_to_ppm();

    private:
        const int img_width{256};
        const int img_height{256};

};


#endif
