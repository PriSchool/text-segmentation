#include "Binarization.hpp"
#include "CropScanner.hpp"
#include "LineSegmentation.hpp"
#include <opencv2/core/utils/filesystem.hpp>

using namespace cv::utils::fs;

int main(int argc, char *argv[]) {

    string src_path = argv[1], out_path = argv[2];
    string name = argv[3], extension = argv[4];
    string src_base = (out_path + name);

    bool illumination = argv[5];
    int threshold_method = stoi(argv[6]);

    string lines_path = join(out_path, "lines");
    string words_path = join(out_path, "words");

    // Mat image = imread(src_path);
    // Mat image = imread(src_path);
    Mat image = imread(src_path, IMREAD_GRAYSCALE);

    int new_w = 1920;
    int new_h = ((new_w * image.rows) / image.cols);
    resize(image, image, Size(new_w, new_h));

    createDirectory(out_path);
    imwrite(src_base + extension, image);

    // 1: binarization
    Binarization *threshold = new Binarization();    
    threshold->binarize(image, image, illumination, threshold_method);

    imwrite(src_base + "_1_binary" + extension, image);

    // 2: crop
    CropScanner *crop = new CropScanner();
    crop->process(image, image, src_base, extension);

    imwrite(src_base + "_2_crop" + extension, image);

    // 3: line segmentation
    LineSegmentation *line = new LineSegmentation();
    vector<cv::Mat> lines;

    line->segment(image, lines, src_base, extension);
    
    createDirectory(lines_path);
    for (int i=0; i< lines.size(); i++) {
        string number = to_string((i+1)*1e-6).substr(5);
        imwrite(join(lines_path, "line_" + number + extension), lines[i]);
    }

    // word segmentation
    // createDirectory(words_path);



    return 0;
}