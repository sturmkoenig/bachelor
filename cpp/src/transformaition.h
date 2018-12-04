
class hilberttransform
{
protected:
	int offset_x;
	int offset_y;
	phasemap pm;
public:
	void measure_offset();
	void transform() override; 
};


void transform(arma::mat data)
{

	phasemap = arma::ifft2( - arma::exp( * sign(w) * arma::fft2(data) );
}
