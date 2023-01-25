#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <windows.h>
#include <windows.h>

using namespace cv;
using namespace std;




int main(int argc, char** argv)
{
    VideoCapture cap(0); //capturer la vidéo de la webcam

    if (!cap.isOpened())  // si ce n'est pas le cas, quittez le programme
    {
        cout << "Cannot open the web cam" << endl;
        return -1;
    }




    int iLowH = 170;
    int iHighH = 179;

    int iLowS = 150;
    int iHighS = 255;

    int iLowV = 60;
    int iHighV = 255;
    Mat frame2;


    int iLastX = -1;
    int iLastY = -1;

    //Capturer une image temporaire à partir de l'appareil photo
    Mat imgTmp;
    cap.read(imgTmp);

    //Créer une image noire avec la taille comme sortie de la caméra
    Mat imgLines = Mat::zeros(imgTmp.size(), CV_8UC3);;

    std::srand(std::time(nullptr));
    int random_variable1 = std::rand() % 480;
    int random_variable2 = std::rand() % 480;
    int random_variable3 = std::rand() % 480;

    int xballon1 = 500;
    int xballon2 = 570;
    int xballon3 = 650;
    int speed1 = 1;
    int speed2 = 1;
    int speed3 = 1;
    int cmt = 0;

    int temps = 15;

    while (true)
    {
        Mat imgOriginal;
        bool bSuccess = cap.read(imgOriginal); // lire une nouvelle image de la vidéo








        if (!bSuccess) // si ce n'est pas réussi, rompre la boucle
        {
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }

        Mat imgHSV;

        cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convertir l'image capturée de BGR en HSV

        Mat imgThresholded, imgThresholded2, imgThresholded3;

        //  inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

        inRange(imgHSV, Scalar(0, 160, 160), Scalar(10, 255, 255), imgThresholded);
        inRange(imgHSV, Scalar(170, 160, 160), Scalar(180, 255, 255), imgThresholded2);
        // inRange(imgHSV, Scalar(94, 80, 2), Scalar(126, 255, 255), imgThresholded3);
        frame2 = imgThresholded | imgThresholded2;

        //morphological opening (removes small objects from the foreground)
        erode(frame2, frame2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        dilate(frame2, frame2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

        flip(imgOriginal, imgOriginal, 1);
        flip(frame2, frame2, 1);

        //Calculer les moments de l'image seuillée
        Moments oMoments = moments(frame2);

        double dM01 = oMoments.m01;
        double dM10 = oMoments.m10;
        double dArea = oMoments.m00;


        //calculate the position of the ball
        int posX = dM10 / dArea;
        int posY = dM01 / dArea;


        circle(imgOriginal, Point(posX, posY), 30, Scalar(255, 0, 0), -1, 10, 0);

        circle(imgOriginal, Point(random_variable1, xballon1), 30, Scalar(255, 0, 0), -1, 10, 0);
        circle(imgOriginal, Point(random_variable2, xballon2), 30, Scalar(0, 0, 255), -1, 10, 0);
        circle(imgOriginal, Point(random_variable3, xballon3), 30, Scalar(0, 250, 0), -1, 10, 0);


        if ((posX <= random_variable1 + 30 && posX >= random_variable1 - 30) && (posY <= xballon1 + 30 && posY >= xballon1 - 30))
        {

            circle(imgOriginal, Point(random_variable1, xballon1), 30, Scalar(0, 0, 255), -1, 10, 0);
            // cout << '\7';
            Beep(200, 200);

            xballon1 = 600;
            cmt++;
            random_variable1 = std::rand() % 480;
            speed1 += 2;

        }


        if ((posX <= random_variable2 + 30 && posX >= random_variable2 - 30) && (posY <= xballon2 + 30 && posY >= xballon2 - 30))
        {

            circle(imgOriginal, Point(random_variable2, xballon2), 30, Scalar(0, 0, 255), -1, 10, 0);
            // cout << '\7';
            Beep(200, 200);

            xballon2 = 600;
            cmt++;
            random_variable1 = std::rand() % 480;
            speed2 += 2;

        }


        if ((posX <= random_variable3 + 30 && posX >= random_variable3 - 30) && (posY <= xballon3 + 30 && posY >= xballon3 - 30))
        {

            circle(imgOriginal, Point(random_variable3, xballon3), 30, Scalar(0, 0, 255), -1, 10, 0);
            // cout << '\7';
            Beep(200, 200);

            xballon3 = 600;
            cmt++;
            random_variable1 = std::rand() % 480;
            speed3 += 2;

        }
        stringstream ss;
        ss << cmt;
        string str = ss.str();
        string score = "SCORE : ";
        score += str;



        if (xballon1 < 0 || xballon2 < 0 || xballon3 < 0)
        {
            putText(imgOriginal, "Game Over ", Point(150, 200),
                FONT_HERSHEY_SIMPLEX, 2.1,
                Scalar(0, 0, 255), 2, LINE_AA);

        }

        putText(imgOriginal, score, Point(50, 50),
            FONT_HERSHEY_SIMPLEX, 2.1,
            Scalar(255, 0, 0), 2, LINE_AA);


        xballon1 -= speed1;
        xballon2 -= speed2;
        xballon3 -= speed3;
        temps--;




        //imshow("Thresholded Image", frame2); //show the thresholded image
        imgOriginal = imgOriginal + imgLines;


        imshow("Original", imgOriginal); //show the original image

        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        {
            cout << "esc key is pressed by user" << endl;
            break;
        }
    }

    return 0;
}