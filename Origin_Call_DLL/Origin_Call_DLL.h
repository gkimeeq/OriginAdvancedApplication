#ifdef _OWIN64  // ��������жϵ�ǰOrigin�ǲ���64λ��
#pragma dll(Origin_Call_DLL_64, header) 
#else
#pragma dll(Origin_Call_DLL_32, header)
#endif  // _OWIN64

// ����Բ����
double GetPi();

// Բ
typedef struct _tagCircle {
	double cx;
	double cy;
	double radius;
} Circle;

// ͨ��ָ�뷵�ؽṹ��
void GetCircle(Circle *pCircle);