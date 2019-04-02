#ifndef TIMERCLASS_HPP_INCLUDED
#define TIMERCLASS_HPP_INCLUDED

namespace ven
{

class Timer
{
    public:
        Timer(  );
        ~Timer(  );

        void  SetTimeScale( float _timeScale ) { this->timeScale = _timeScale; }

        float GetDeltaTime(  ) { return this->deltaTime; }
        float GetTimeScale(  ) { return this->timeScale; }

        void Update(  );

    private:
        unsigned int timeStart;
        unsigned int timeElapsed;
        float deltaTime;
        float timeScale;

};

}

#endif // TIMERCLASS_HPP_INCLUDED
