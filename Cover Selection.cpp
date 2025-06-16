#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// Function to compute histogram for an image
Mat computeHistogram(const Mat& image) {
    vector<Mat> bgr_planes;
    split(image, bgr_planes); // Split into B, G, R channels

    int histSize = 256;
    float range[] = {0, 256};
    const float* histRange = {range};

    Mat b_hist, g_hist, r_hist;
    calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange);
    calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange);
    calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange);

    return b_hist + g_hist + r_hist;
}

// Function to encode binary text into an image (LSB Method)
void encodeTextInImage(Mat& image, const string& text) {
    string binaryText = "";
    for (char c : text) {
        bitset<8> bits(c);
        binaryText += bits.to_string();
    }

    int pixelIndex = 0;
    int rows = image.rows, cols = image.cols;

    for (int r = 0; r < rows && pixelIndex < binaryText.size(); r++) {
        for (int c = 0; c < cols && pixelIndex < binaryText.size(); c++) {
            Vec3b& pixel = image.at<Vec3b>(r, c);
            for (int channel = 0; channel < 3 && pixelIndex < binaryText.size(); channel++) {
                char bit = binaryText[pixelIndex++];
                unsigned char& pixelValue = pixel[channel];

                if (bit == '0' && pixelValue % 2 != 0) pixelValue -= 1;
                if (bit == '1' && pixelValue % 2 == 0) pixelValue += 1;
            }
        }
    }
}

// Function to compare histograms
double compareHistograms(const Mat& hist1, const Mat& hist2) {
    return compareHist(hist1, hist2, HISTCMP_CHISQR);
}

int main() {
    vector<string> messages = {
        "Clouds gather before the storm.",
        "Access Key: #S7e9@Tx!",
        "The blue fox crosses the river.",
        "Target location: Grid A3, Sector 7",
        "Trust no one but the raven."
    };

    vector<string> coverImages = {"1.png", "2.png", "3.png", "4.png", "5.png", "6.png", "7.png", "8.png", "9.png"};

    for (size_t i = 0; i < messages.size(); i++) {
        string bestImage;
        double minDifference = DBL_MAX;

        cout << "\nFinding best cover for message " << i + 1 << "...\n";

        for (size_t j = 0; j < coverImages.size(); j++) {
            Mat image = imread(coverImages[j]);
            if (image.empty()) {
                cerr << "Could not open image: " << coverImages[j] << endl;
                continue;
            }

            Mat originalHist = computeHistogram(image);
            encodeTextInImage(image, messages[i]);
            Mat modifiedHist = computeHistogram(image);
            double difference = compareHistograms(originalHist, modifiedHist);

            cout << "  - Histogram difference with " << coverImages[j] << ": " << difference << endl;

            if (difference < minDifference) {
                minDifference = difference;
                bestImage = coverImages[j];
            }
        }

        cout << "Best cover for Message " << i + 1 << ": " << bestImage << " (Histogram difference: " << minDifference << ")\n";
    }

    return 0;
}
