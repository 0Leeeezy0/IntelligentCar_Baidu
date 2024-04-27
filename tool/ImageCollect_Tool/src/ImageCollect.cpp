#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

typedef enum
{
    BRIDGE = 0,
    CROSSWALK = 1,
    EVIL = 2,
    CONE = 3,
    BLOCK = 4,
    BOMB = 5,
    HYBIRD = 6,
}ImageKind;

int main()
{
    VideoCapture Camera; // 定义相机
    Mat Img;
    ImageKind ImgKind;
    string ImgWritePath;
    Size size = Size(320,240);
    int ImgKind_int;
    int ImgNum = 0;

    Camera.open(0);
    cout << "0 bridge\n1 crosswalk\n2 evil\n3 cone\n4 block\n5 bomb\n6 hybird" << endl;
    cout << "IMAGE KIND OF COLLECT:";
    cin >> ImgKind_int;
    ImgKind = ImageKind(ImgKind_int);

    while(1)
    {
        
        Camera >> Img;
		resize(Img,Img,size,0,0,INTER_AREA);
        switch(ImgKind)
        {
            case BRIDGE:{ ImgWritePath = "../img/bridge/" + to_string(ImgNum) + ".jpg"; break; }
            case CROSSWALK:{ ImgWritePath = "../img/crosswalk/" + to_string(ImgNum) + ".jpg"; break; }
            case EVIL:{ ImgWritePath = "../img/evil/" + to_string(ImgNum) + ".jpg"; break; }
            case CONE:{ ImgWritePath = "../img/cone/" + to_string(ImgNum) + ".jpg"; break; }
            case BLOCK:{ ImgWritePath = "../img/block/" + to_string(ImgNum) + ".jpg"; break; }
            case BOMB:{ ImgWritePath = "../img/bomb/" + to_string(ImgNum) + ".jpg"; break; }
            case HYBIRD:{ ImgWritePath = "../img/hybird/" + to_string(ImgNum) + ".jpg"; break; }
        }
        imshow("COLLECT",Img);
        imwrite(ImgWritePath,Img);
        ImgNum++;
        waitKey(1);
    }
    return 0;
}