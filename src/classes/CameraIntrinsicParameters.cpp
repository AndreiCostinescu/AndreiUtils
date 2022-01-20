#include <AndreiUtils/classes/CameraIntrinsicParameters.h>

using namespace AndreiUtils;
using namespace std;

CameraIntrinsicParameters::CameraIntrinsicParameters()
                : h(), w(), fx(), fy(), ppx(), ppy(), distortionModel(ImageDistortionModel::DISTORTION_NONE),
                  distortionCoefficients(nullptr), nrDistortionCoefficients(0) {}

		  CameraIntrinsicParameters::CameraIntrinsicParameters(int h, int w, double fx, double fy, double ppx, double ppy,
                                  ImageDistortionModel distortionModel, const float *distortionCoefficients) :
                h(h), w(w), fx(fx), fy(fy), ppx(ppx), ppy(ppy), distortionModel(distortionModel) {
            this->nrDistortionCoefficients = getNumberOfDistortionCoefficients(distortionModel);
            this->distortionCoefficients = new float[this->nrDistortionCoefficients];
            for (int i = 0; i < this->nrDistortionCoefficients; i++) {
                this->distortionCoefficients[i] = distortionCoefficients[i];
            }
        }

CameraIntrinsicParameters::~CameraIntrinsicParameters() {
            delete[] this->distortionCoefficients;
        }

        string CameraIntrinsicParameters::toString() const {
            string s;
            s += "(h, w) = (" + to_string(h) + ", " + to_string(w) + ")\n";
            s += "(fx, fy, ppx, ppy) = (" + to_string(fx) + ", " + to_string(fy) + ", " +
                 to_string(ppx) + ", " + to_string(ppy) + ")\n";
            s += "distortion model = " + convertImageDistortionModelToString(this->distortionModel) + "\n";
            s += "distortion coefficients = {";
            for (int i = 0; this->distortionCoefficients == nullptr || i < this->nrDistortionCoefficients; i++) {
                if (i > 0) {
                    s += ", ";
                }
                s += to_string(this->distortionCoefficients[i]);
            }
            s += "}\n";
            return s;
        }

        void CameraIntrinsicParameters::setImageParameters(int _h, int _w, double _fx, double _fy, double _ppx, double _ppy) {
            this->h = _h;
            this->w = _w;
            this->fx = _fx;
            this->fy = _fy;
            this->ppx = _ppx;
            this->ppy = _ppy;
        }

void CameraIntrinsicParameters::setDistortionParameters(ImageDistortionModel _distortionModel, const float *_distortionCoefficients) {
            this->distortionModel = _distortionModel;
            delete[] this->distortionCoefficients;
            this->distortionCoefficients = nullptr;
            this->nrDistortionCoefficients = getNumberOfDistortionCoefficients(_distortionModel);
            if (this->nrDistortionCoefficients == 0 || _distortionCoefficients == nullptr) {
                return;
            }
            this->distortionCoefficients = new float[this->nrDistortionCoefficients];
            for (int i = 0; i < this->nrDistortionCoefficients; i++) {
                this->distortionCoefficients[i] = _distortionCoefficients[i];
            }
        }

        void CameraIntrinsicParameters::setDistortionParameters(ImageDistortionModel _distortionModel,
                                     const std::vector<float> &_distortionCoefficients) {
            this->distortionModel = _distortionModel;
            delete[] this->distortionCoefficients;
            this->nrDistortionCoefficients = getNumberOfDistortionCoefficients(_distortionModel);
            this->distortionCoefficients = new float[this->nrDistortionCoefficients];
            for (int i = 0; i < this->nrDistortionCoefficients; i++) {
                this->distortionCoefficients[i] = _distortionCoefficients[i];
            }
        }

vector<float> CameraIntrinsicParameters::getDistortionCoefficientsAsVector() const {
    vector<float> coefficients(this->nrDistortionCoefficients);
    for (int i=0; i<this->nrDistortionCoefficients; i++) {
        coefficients[i] = this->distortionCoefficients[i];
    }
    return coefficients;
}


