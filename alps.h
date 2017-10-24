

class Alps
{
public:
	void MotorBegin(int pwm, int up, int down, int cycle , long freq, int et);
	int Read(int pin, int mode);
	int Move(int val);
	void TouchBegin(int tpin1, int tpin2);
	int Touched(int ms);
private:
	void MotorUp();
	void MotorDown();
	void MotorStop();

};
