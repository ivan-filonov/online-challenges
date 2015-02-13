#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>

using namespace std;

void cmyk(string s) {
	istringstream ss {s};
	char z;
	double c,m,y,k;
	ss >> z  >> c >> z >> m >> z >> y >> z >> k >> z;
	auto coeff = 255.0 * (1.0-k);
	cout << lround( coeff * (1 - c)) << ',' << lround(coeff * (1-m)) << ',' << lround(coeff*(1-y));
}

void hex(string s) {
	auto v = std::stoi(s.substr(1), nullptr, 16);
	cout << (255&(v>>16)) << ',' << (255&(v>>8)) << ',' << (255&v);
}

void hsv(string sp) {
	istringstream ss {sp};
	char z;
	double h,s,v;
	ss.ignore(4);
	ss >> h >> z >> s >> z >> v >> z;
	auto vmin=v * (100.0-s) * .01;
	auto a = (v-vmin)*(((int)h%60)/60.0);
	auto vinc = vmin + a;
	auto vdec = v - a;
	double r,g,b;
	switch((int)std::floor(h/60)) {
		case 0:	tie(r,g,b)=make_tuple(	v, 	vinc,	vmin);	break;
		case 1:	tie(r,g,b)=make_tuple(	vdec,	v,	vmin);	break;
		case 2:	tie(r,g,b)=make_tuple(	vmin,	v,	vinc);	break;
		case 3:	tie(r,g,b)=make_tuple(	vmin,	vdec,	v);	break;
		case 4:	tie(r,g,b)=make_tuple(	vinc,	vmin,	v);	break;
		case 5:	tie(r,g,b)=make_tuple(	v,	vmin,	vdec);	break;
	}
	cout << lround(r*2.55) << ',' << lround(g*2.55) << ',' << lround(b*2.55);
}

void hsl(string str) {
	istringstream ss {str};
	char z;
	double h,s,l;
	ss.ignore(4);
	ss >> h >> z >> s >> z >> l >> z;
	s *= .01;
	l *= .01;
	auto q = (l < .5) ? l * (1.0 + s) : (l + s - l * s);
	auto p = 2.0 * l - q;
	auto hk = h/360.0;
	auto F1 = [](double x){
		if(x < 0.0) {
			return x + 1.0;
		}
		if( x > 1.0) {
			return x - 1.0;
		}
		return x;
	};
	auto tr = F1(hk + 1.0/3.0);
	auto tg = F1(hk);
	auto tb = F1(hk - 1.0/3.0);
	auto color = [=](double x) {
		double res;
		if(x < 1.0/6.0) {
			res = p + ((q-p)*6.0*x);
		}
		else if(x < .5) {
			res = q;
		}
		else if(x < 2.0/3.0) {
			res = p + ((q-p)*(2.0/3.0-x)*6.0);
		}
		else {
			res = p;
		}
		return lround(255.0 * res);
	};
	auto r = color(tr);
	auto g = color(tg);
	auto b = color(tb);
	cout << r << ',' << g << ',' << b;
}

void process(string line) {
	cout << "RGB(";
	if('(' == line[0]) {
		cmyk(line);
	}
	else if('#' == line[0]) {
		hex(line);
	}
	else if('H' == line[0]) {
		if('L' == line[2]) {
			hsl(line);
		}
		else if('V' == line[2]) {
			hsv(line);
		}
		else throw "bad arg";
	}
	else throw "bad arg";
	cout << ')' << endl;
}

int main(int _c, char** _v) {
	ifstream stream(_v[1]);
	string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

