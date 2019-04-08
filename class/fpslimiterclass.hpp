#ifndef FPSLIMITERCLASS_HPP_INCLUDED
#define FPSLIMITERCLASS_HPP_INCLUDED

namespace ven
{

class FPSLimiter
{
    public:
        FPSLimiter( unsigned int _fpslimit = 30 );
        ~FPSLimiter(  );

        void SetFrameRateLimit( unsigned int _fpslimit );

        void UpdateStart(  );
        void UpdateEnd(  );

    private:
        unsigned int fpslimit;
        unsigned int cycle;
};

}

#endif // FPSLIMITERCLASS_HPP_INCLUDED
