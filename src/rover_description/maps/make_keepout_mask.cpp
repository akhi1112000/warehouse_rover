#include <vector>
#include <fstream>

int main()
{
    const int W= 400;
    const int H= 400;

    const double RES = 0.05;
    const double ORIGIN_X = -6.975;
    const double ORIGIN_Y = -6.969;

    const double SHELF_LENGTH = 3.68;
    const double SHELF_WIDTH = 0.65;

    const double MARGIN = 0.15;
    const double SHELF_X = 2.55;
    const std::vector<double> SHELF_YS = {-1.30, 0.85, 3.00, 5.15, 7.30};
    std::vector<unsigned char> pixels(W * H, 255);

        for (double y_center : SHELF_YS)
    {
        const double x_min = SHELF_X - SHELF_LENGTH / 2 - MARGIN;
        const double x_max = SHELF_X + SHELF_LENGTH / 2 + MARGIN;
        const double y_min = y_center - SHELF_WIDTH / 2 - MARGIN;
        const double y_max = y_center + SHELF_WIDTH / 2 + MARGIN;

                for (int row = 0; row < H; row++)
        {
            const double y = ORIGIN_Y + (H - row - 0.5) * RES;

            for (int col = 0; col < W; col++)
            {
                const double x = ORIGIN_X + (col + 0.5) * RES;

                if (x >= x_min && x <= x_max && y >= y_min && y <= y_max)
                {
                    pixels[row * W + col] = 0;
                }
            }
        }
    }

     std::ofstream file("keepout_mask.pgm", std::ios::binary);
    file << "P5\n" << W << " " << H << "\n255\n";
    file.write(reinterpret_cast<const char*>(pixels.data()), pixels.size());



    
    return 0;
}