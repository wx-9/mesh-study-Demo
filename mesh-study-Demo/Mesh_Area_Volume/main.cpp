#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <cstring>
#include <math.h>
using namespace std;
struct Points{  // 顶点坐标
    float num_x;
    float num_y;
    float num_z;
};
struct Face_Index{
    int fri_point;
    int sec_point;
    int thi_point;
};
struct Faces{
    Face_Index points_index; // 一个面上的三个点的坐标索引

    Face_Index normal_index; // 一个面上的 三个法向量索引

};
//  顶点坐标，法向量，面中对应的index， 分别放到了一个迭代器中
vector<Points> ve_points;
vector<Points> normal_vectors;
vector<Faces> faces_index;

vector<string> split(const string& str, const string& delim) {
    vector<string> res;
    if ("" == str) return res;
    //先将要切割的字符串从string类型转换为char*类型
    char *strs = new char[str.length() + 1]; //不要忘了
    strcpy(strs, str.c_str());

    char *d = new char[delim.length() + 1];
    strcpy(d, delim.c_str());

    char *p = strtok(strs, d);
    while (p) {
        string s = p; //分割得到的字符串转换为string类型
        res.push_back(s); //存入结果数组
        p = strtok(NULL, d);
    }

    return res;
}
// 读取obj文件，将顶点坐标 法向量 以及面上的顶点索引 分别放到迭代器中。
int read_obj(string path)
{
    ifstream file;
    file.open(path);
    string filename;
    string line;
    char head;
    char head2;

    if(!file)
        return -1;

    while (getline (file, line)) // line中不包括每行的换行符
    {
        int length = line.length();
        if(length >=2)
        {
            head = line[0];
            head2 = line[1];
            if(head == 'v' and head2 == ' ') // 添加mesh坐标
            {

                Points one_point;
                vector<string> res = split(line," ");
                one_point.num_x = stof(res[1]);
                one_point.num_y = stof(res[2]);
                one_point.num_z = stof(res[3]);
                ve_points.push_back(one_point);
            }
            if(head == 'v' and head2 == 'n')  //添加法向量
            {
                Points one_normal;
                vector<string> res = split(line," ");
                one_normal.num_x = stof(res[1]);
                one_normal.num_y = stof(res[2]);
                one_normal.num_z = stof(res[3]);
                normal_vectors.push_back(one_normal);
            }
            if(head == 'f' and head2 == ' ')  //添加法向量
            {
                //Points face;20
                Faces face;
                vector<string> res = split(line," ");
                vector<string> index_1 = split(res[1],"/");
                vector<string> index_2 = split(res[2],"/");
                vector<string> index_3 = split(res[3],"/");
                face.points_index.fri_point = stoi(index_1[0])-1; //obj文件中索引从1开始 坐标在迭代器中从0开始
                face.points_index.sec_point = stoi(index_2[0])-1;
                face.points_index.thi_point = stoi(index_3[0])-1;
                face.normal_index.fri_point = stoi(index_1[2])-1;
                face.normal_index.sec_point = stoi(index_2[2])-1;
                face.normal_index.thi_point = stoi(index_3[2])-1;

                faces_index.push_back(face);
            }

        }
    }
    cout << "get obj information ,over!" <<endl;
    return 0;
}
//打印 获取到的信息
int print_infor()
{

    for(int i = 0;i<ve_points.size();++i)
    {
        cout << "第" << i << "个点坐标  ：";
        cout << "X:"<<ve_points[i].num_x << "、";
        cout << "Y:"<< ve_points[i].num_y << "、";
        cout << "Z:"<<ve_points[i].num_z << endl;
    }
    for(int i = 0;i<faces_index.size();++i)
    {
        cout << "第" << i << "个面 三个顶点的索引值：";
        cout << faces_index[i].points_index.fri_point << "、";
        cout << faces_index[i].points_index.sec_point  << "、";
        cout << faces_index[i].points_index.thi_point << endl;
    }
    for(int i = 0;i<faces_index.size();++i)
    {
        cout << "第" << i << "个面 三个顶点的坐标：";
        int frist =  faces_index[i].points_index.fri_point;
        int second = faces_index[i].points_index.sec_point;
        int thrid = faces_index[i].points_index.thi_point;
        cout<<'('<< ve_points[frist].num_x<<','<< ve_points[frist].num_y<<','<< ve_points[frist].num_z<<");";
        cout<<'('<< ve_points[second].num_x<<','<< ve_points[second].num_y<<','<< ve_points[second].num_z<<");";
        cout<<'('<< ve_points[thrid].num_x<<','<< ve_points[thrid].num_y<<','<< ve_points[thrid].num_z<<");"<<endl;

    }


    return 0;
}

//叉乘公式  两个向量叉乘 即为两个向量组成的平行四边形的面积 除以2及时组成的三角形的面积
double triangle_area(float p1X, float p1Y, float p1Z,
                     float p2X, float p2Y, float p2Z,
                     float p3X, float p3Y, float p3Z)
{
    double ax = p2X - p1X;
    double ay = p2Y - p1Y;
    double az = p2Z - p1Z;
    double bx = p3X - p1X;
    double by = p3Y - p1Y;
    double bz = p3Z - p1Z;
    double cx = ay*bz - az*by;
    double cy = az*bx - ax*bz;
    double cz = ax*by - ay*bx;
    return (double)(0.5 * sqrt(cx*cx + cy*cy + cz*cz));
}

//计算四面体的公式  https://zhuanlan.zhihu.com/p/26140241
double triangle_volume(float p1X, float p1Y, float p1Z,
                       float p2X, float p2Y, float p2Z,
                       float p3X, float p3Y, float p3Z)
{
    double v321 = p3X*p2Y*p1Z;
    double v231 = p2X*p3Y*p1Z;
    double v312 = p3X*p1Y*p2Z;
    double v132 = p1X*p3Y*p2Z;
    double v213 = p2X*p1Y*p3Z;
    double v123 = p1X*p2Y*p3Z;
    return (double)(1.0/6.0)*(-v321 + v231 + v312 - v132 - v213 + v123);
}

vector<double> mesh_surface_volume()
{
    double area = 0.0;
    double volume = 0.0;
    vector<double> results;
    for(int i = 0;i<faces_index.size();++i)
    {
        int frist =  faces_index[i].points_index.fri_point;
        int second = faces_index[i].points_index.sec_point;
        int thrid = faces_index[i].points_index.thi_point;
        float p1x = ve_points[frist].num_x;
        float p1y = ve_points[frist].num_y;
        float p1z = ve_points[frist].num_z;
        float p2x = ve_points[second].num_x;
        float p2y = ve_points[second].num_y;
        float p2z = ve_points[second].num_z;
        float p3x = ve_points[thrid].num_x;
        float p3y = ve_points[thrid].num_y;
        float p3z = ve_points[thrid].num_z;
        area += triangle_area(p1x,p1y,p1z,p2x,p2y,p2z,p3x,p3y,p3z);
        volume += triangle_volume(p1x,p1y,p1z,p2x,p2y,p2z,p3x,p3y,p3z);
        //cout<< area << endl;

    }
    results.push_back(area);
    results.push_back(volume);
    return results;
}




int main()
{
    cout.setf(ios_base::fixed,ios_base::floatfield); // 能够让double float输出完整 不然只能输出六位有效数字
    string obj_path = "./test.obj";
    int res = read_obj(obj_path);
    if(res<0)
    {cout<<"文件读取错误！！"<< endl; return 0; }

    // 输出信息
    //print_infor();
    //计算表面积
    vector<double> result = mesh_surface_volume();
    double Area = result[0];
    double Volume = result[1];
    cout<< "该3D模型表面积为："<<Area <<endl;
    cout<< "该3D模型体积为："<<Volume <<endl;

    return 0;
}

