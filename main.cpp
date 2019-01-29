#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
using namespace std;
ofstream fout;

class Point
{
    public:
        Point();
        Point(double h,double v);
        friend double Distance(const Point& start,const Point& end);
        friend double slope(const Point& start, const Point& end);
        friend double perpen_slope(const Point& start, const Point& end);
        friend Point plotBySlopLenght(const Point& start,double length, double slope,bool upward);
        friend Point plothillfoot(const Point& start,const Point& end,double length);
        friend bool UpDown(const Point& start,const Point& down);
        double x;
        double y;
};
bool UpDown(const Point &start, const Point &end, double slope);
void fractal(const Point &start, const Point &end, const double &length);
double Round(double num);

int main()
{
    system("clear");
    Point A(200,500),B(600,500);
    fout.open("coord.svg");
    fout << "<?xml version=\"1.0\" standalone=\"no\"?>\n"
            "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n"
            "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n"
            "<svg width=\"5000\" height=\"5000\"\n"
            "xmlns=\"http://www.w3.org/2000/svg\">\n\n";


    int esp=1;
    fractal(B,A,esp);

    fractal(A,Point(400,840),esp);
    fractal(Point(400,840),B,esp);
    fout << "</svg>";

    return 0;
}

Point::Point():x(0.0),y(0.0)
{
}
Point::Point(double h, double v):x(h),y(v)
{
}
double Distance(const Point &start,const Point& end)
{
    double distance;
     distance= pow((pow(start.x-end.x,2)+pow(start.y-end.y,2)),0.5);
     return distance;
}
double slope(const Point& start, const Point& end)
{
    double s;
    try
    {
        if(start.x-end.x==0)
            throw 1;
        s=(start.y-end.y)/(start.x-end.x);
        return s;
    }
    catch (int)
    {
        return 9999;
    }

}
double perpen_slope(const Point& start, const Point& end)
{
    try
    {
        double s;
        s=slope(start,end);
        if(s==0)
            throw 1;
        s=-1/s;
        return s;
    }
    catch (int)
    {
        return 9999;
    }
}
Point plotBump(const Point& start,double length, double slope,bool upward)
{
    Point temp;
    try
    {

        if(slope==9999)
        {
            throw 1;
        }

        double angle;
        angle=Round(atan(slope));
//        cout <<"angle "<<angle/3.14*180<<"BUMP"<<endl;
        if ((upward&& angle>0)||( (!upward)&& angle<0))
        {
//            cout <<"x="<<start.x <<"+"<<length*cos(angle)<<endl;
            temp.x=start.x+length*cos(angle);
//            cout <<"y="<<start.y <<"+"<<length*sin(angle)<<endl;
            temp.y=start.y+length*sin(angle);
        }
        if (((!upward)&& angle>0)||(upward&&angle<0))
        {
//             cout <<"x="<<start.x <<"-"<<length*cos(angle)<<endl;
            temp.x=start.x-length*cos(angle);
//            cout <<"y="<<start.y <<"-"<<length*sin(angle)<<endl;
            temp.y=start.y-length*sin(angle);
        }
        if(angle==0)
        {
            temp.y=start.y;
            if(upward)
            {
                temp.x=start.x+length;
            }
            else
            {
                temp.x=start.x-length;
            }
        }
    }
    catch (int )
    {
//        cout <<"vertical slope"<<endl;
        temp.x=start.x;
        if (upward)
        {
            temp.y=start.y+length;
        }
        else
        {
            temp.y=start.y-length;
        }
    }
    return temp;
}
Point plothillfoot(const Point& start,const Point& end,double length)
{
    Point temp;
    double s=slope(start,end);
    try
    {
        if(s==9999)
        {
            throw 1;
        }
        double angle=atan(s);
//        cout <<"angle "<<angle/3.14*180<<"plotfoot"<<endl;
        if ((s<0 && end.y>start.y)||(s>0&& end.y<start.y))
        {
//            cout<<"1";
//            cout <<"x="<<start.x <<"-"<<length*cos(angle)<<endl;
            temp.x=start.x-cos(angle)*length;
//            cout <<"y="<<start.y <<"-"<<length*sin(angle)<<endl;
            temp.y=start.y-length*sin(angle);
        }
        else if ((s<0 && end.y<start.y)||(s>0&& end.y>start.y))
        {
//            cout<<"2";
//            cout <<"x="<<start.x <<"+"<<length*cos(angle)<<endl;
            temp.x=start.x+length*cos(angle);
//            cout <<"y="<<start.y <<"+"<<length*sin(angle)<<endl;
            temp.y=start.y+length*sin(angle);
        }
        else if (s==0)
        {
            temp.y=start.y;
            if (start.x<end.x)
            {
//                cout<<"3";
                temp.x=start.x+length;
            }
            else
            {
//                cout<<"4";
                temp.x=start.x-length;
            }
        }

    }
    catch (int)
    {
//        cout <<"vertical slope"<<endl;
        temp.x=start.x;
        if(start.y>end.y)
        {
            temp.y=start.y-length;
        }
        else
        {
            temp.y=start.y+length;
        }
    }
    return temp;
}
bool UpDown(const Point& start,const Point& end,double slope)
{
    if(slope <0)
    {
        if(end.y>start.y)
            return 0;
        else
            return 1;
    }
    else if(slope > 0)
    {
        if(end.y>start.y)
            return 1;
        else
            return 0;
    }
    else
    {
        if(start.x<end.x)
            return 1;
        else
            return 0;
    }
}

void fractal(const Point &start, const Point& end, const double &length)
{
    double D=Distance(start,end);
    double segment=Round(D/3.0);

    if (segment>length)
    {
        double line_slope=slope(start,end);
        double perpenSlope=perpen_slope(start,end);
        bool up=UpDown(start,end,line_slope);
        Point P_one_third,P_two_thirds;
        Point Mid;
        Point Bump;
//        if(up)
//            cout<<"UP\n";
//        else
//            cout<<"DOWN\n";
        Mid.x=(start.x+end.x)/2.0;
        Mid.y=(start.y+end.y)/2.0;

//        cout <<"plot 1/3: ";
        P_one_third=plothillfoot(start,end,segment);
//        cout <<"plot 2/3: ";
        P_two_thirds=plothillfoot(start,end,segment*2.0);
//        cout <<"plot bump: ";
        Bump=plotBump(Mid,segment*sqrt(3.0)/2.0,perpenSlope,up);

//        cout <<"Mid: "<< Mid.x<<","<<Mid.y<<endl;
//        cout <<"BUMP: "<<Bump.x<<","<<Bump.y<<endl;
//        cout <<"1/3p: "<<P_one_third.x<<","<<P_one_third.y<<endl;
//        cout <<"2/3p: "<<P_two_thirds.x<<","<<P_two_thirds.y<<endl;

        fractal(start,P_one_third,length);
        fractal(P_one_third,Bump,length);
        fractal(Bump,P_two_thirds,length);
        fractal(P_two_thirds,end,length);
    }
    else
    {
//        cout <<"DIst"<<segment<<endl;
        cout.setf(ios::fixed);
        cout.setf(ios::showpoint);
        cout.setf(ios::left);
        cout.precision(5);
        cout<<setw(8) <<start.x << " " <<start.y<<endl;
        cout<<setw(8) << end.x << " " <<end.y <<endl;
        fout << "<line x1=\""<< start.x <<"\" y1=\""<< start.y<<"\" x2=\""<<end.x<<"\" y2=\""<<end.y<<"\"\n"
                "style=\"stroke:purple;stroke-width:0.5\"/>\n";
    }

}
double Round(double num)
{
    num=num*1000;
    int temp=num;
    if((temp%10)>=5.0)
    {
        num=num/10.0;
        num=ceil(num);

    }
    else
    {
        num=num/10.0;
        num=floor(num);
    }
    num=num/100.0;
    return num;
}
