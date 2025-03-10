#ifndef PERLIN_H
#define PERLIN_H


class Perlin
{
    private:
        static const int pointCount = 256;
        Vector3 randomVectors[pointCount];
        int permX[pointCount];
        int permY[pointCount];
        int permZ[pointCount];

        static void perlinGeneratePerm(int* p)
        {
            for (int i = 0; i < pointCount; i++)
                p[i] = i;
            
            permute(p, pointCount);
        }

        static void permute(int* p, int n)
        {
            for (int i = n-1; i > 0; i--)
            {
                int target = randomInt(0, i);
                int tmp = p[i];
                p[i] = p[target];
                p[target] = tmp;
            }
        }

        static float perlinInterpolation(const Vector3 values[2][2][2], float u, float v, float w)
        {
            float uu = u * u * (3 - 2 * u);
            float vv = v * v * (3 - 2 * v);
            float ww = w * w * (3 - 2 * w);

            float accumulatedValue = 0.0f;

            for (int i = 0; i < 2; i++)
                for (int j = 0; j < 2; j++)
                    for (int k = 0; k < 2; k++)
                    {
                        Vector3 weightVector = Vector3(u - i, v - j, w - k);
                        accumulatedValue += (i * uu + (1 - i) * (1 - uu))
                                          * (j * vv + (1 - j) * (1 - vv))
                                          * (k * ww + (1 - k) * (1 - ww))
                                          * dotP(values[i][j][k], weightVector);
                    }
            
            return accumulatedValue;
        }

    
    public:
        Perlin()
        {
            for (int i = 0; i < pointCount; i++)
                randomVectors[i] = normalized(Vector3::randomVector(-1, 1));
            
            perlinGeneratePerm(permX);
            perlinGeneratePerm(permY);
            perlinGeneratePerm(permZ);
        }


        float noise(const Point3& p) const
        {
            float u = p.x() - std::floor(p.x());
            float v = p.y() - std::floor(p.y());
            float w = p.z() - std::floor(p.z());

            int i = int(std::floor(p.x()));
            int j = int(std::floor(p.y()));
            int k = int(std::floor(p.z()));

            Vector3 values[2][2][2];

            for (int di = 0; di < 2; di++)
                for (int dj = 0; dj < 2; dj++)
                    for (int dk = 0; dk < 2; dk++)
                        values[di][dj][dk] = randomVectors
                        [
                            permX[(i + di) & 255] ^
                            permY[(j + dj) & 255] ^
                            permZ[(k + dk) & 255]
                        ];
                    
            return perlinInterpolation(values, u, v, w);
        }

        float turbulence(const Point3& p, int depth) const
        {
            float accumulatedValue = 0.0f;
            Point3 tmpP = p;
            float weight = 1.0f;

            for (int i = 0; i < depth; i++)
            {
                accumulatedValue += weight * noise(tmpP);
                weight *= 0.5f;
                tmpP *= 2;
            }

            return std::fabs(accumulatedValue);
        }
};


#endif
