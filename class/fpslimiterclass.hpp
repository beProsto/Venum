#ifndef FPSLIMITERCLASS_HPP_INCLUDED
#define FPSLIMITERCLASS_HPP_INCLUDED

namespace ven
{

class FPSLimiter
{
    public:
        FPSLimiter( unsigned int _fpsinvertal = 15 );
        ~FPSLimiter(  );

        void SetFrameRateLimitInvertal( unsigned int _fpsinvertal );

        void  Start(  );
        void Update(  );

    private:
        unsigned int TimeLeft(  );

        unsigned int fpsinvertal;
        unsigned int cycle;
};

}

#endif // FPSLIMITERCLASS_HPP_INCLUDED
