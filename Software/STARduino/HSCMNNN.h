#ifndef PRESSURE_H
#define PRESSURE_H

class HSCMNNN {
    public:
        HSCMNNN(int _sesnsorPin);
        ~HSCMNNN();

        void set_number_samples(int _samples);

    private:
        int sensePin;
        int numSamples;
};

#endif // PRESSURE_H
