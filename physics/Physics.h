#ifndef PHYSICS_H_INCLUDED
#define PHYSICS_H_INCLUDED


class Physics{
    public:
    Physics(double pt, double ft, double ph) : peakTime(pt), fallTime(ft), peakHeight(pt) {
        ascendG = 2 * ph / (pt * pt);
        descendG = 2 * ph / (ft * ft);
        ascendV = 2 * ph / pt * -1;
    }
    double getG(double vy) {
        if (vy >= 0) return descendG;
        return ascendG;
    }
    private:
    double peakTime, fallTime, peakHeight;
    double ascendG, descendG;
    double ascendV;
};

#endif