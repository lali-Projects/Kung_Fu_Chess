#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <utility>


/**
 * @class Img
 * @brief עטיפה כללית מעל cv::Mat.
 *
 * המחלקה אחראית על פעולות בסיסיות של תמונה:
 * טעינה, ציור, כתיבת טקסט ויצירת Canvas.
 *
 * המחלקה אינה מכירה:
 * - משחק
 * - לוח
 * - כלים
 * - GUI
 *
 * היא כלי עזר גרפי כללי.
 */
class Img
{
private:

    cv::Mat img;


public:

    /**
     * @brief בנאי ברירת מחדל.
     *
     * יוצר תמונה ריקה.
     */
    Img();


    /**
     * @brief יצירת Canvas חדש.
     *
     * @param width רוחב התמונה בפיקסלים.
     * @param height גובה התמונה בפיקסלים.
     * @param background צבע הרקע.
     */
    Img(
        int width,
        int height,
        const cv::Scalar& background =
            cv::Scalar(255,255,255,255)
    );


    /**
     * @brief טעינת תמונה מקובץ.
     */
    Img& read(
        const std::string& path,
        const std::pair<int,int>& size = {},
        bool keep_aspect = false,
        int interpolation = cv::INTER_AREA
    );


    /**
     * @brief ציור תמונה זו על תמונה אחרת.
     */
    void draw_on(
        Img& other_img,
        int x,
        int y
    );


    /**
     * @brief כתיבת טקסט על התמונה.
     */
    void put_text(
        const std::string& txt,
        int x,
        int y,
        double font_size,
        const cv::Scalar& color =
            cv::Scalar(255,255,255,255),
        int thickness = 1
    );


    /**
     * @brief הצגת התמונה בחלון.
     *
     * מיועד בעיקר לבדיקות.
     */
    void show();


    /**
     * @brief ניקוי התמונה בצבע נתון.
     */
    void clear(
        const cv::Scalar& color =
            cv::Scalar(255,255,255,255)
    );


    /**
     * @brief ציור מלבן ריק.
     */
    void draw_rectangle(
        int x,
        int y,
        int width,
        int height,
        const cv::Scalar& color,
        int thickness = 1
    );


    /**
     * @brief ציור מלבן מלא.
     */
    void fill_rectangle(
        int x,
        int y,
        int width,
        int height,
        const cv::Scalar& color
    );


    /**
     * @brief ציור קו.
     */
    void draw_line(
        int x1,
        int y1,
        int x2,
        int y2,
        const cv::Scalar& color,
        int thickness = 1
    );


    /**
     * @brief מחזיר רוחב תמונה.
     */
    int width() const;


    /**
     * @brief מחזיר גובה תמונה.
     */
    int height() const;


    /**
     * @brief בדיקה האם התמונה ריקה.
     */
    bool empty() const;


    /**
     * @brief החזרת cv::Mat לקריאה.
     */
    const cv::Mat& get_mat() const
    {
        return img;
    }
};