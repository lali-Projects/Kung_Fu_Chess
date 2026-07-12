#pragma once

/**
 * @class GameConfig
 * @brief מחלקת הגדרות וקבועים מרכזית למניעת מספרי קסם בקוד העסקי.
 */
class GameConfig {
public:
    // גודל פיקסלי של משבצת בלוח (משמש לפענוח קואורדינטות לחיצה)
    static constexpr int CELL_SIZE_PX = 100;
    
    // זמן ברירת המחדל (במילישניות) שלוקח לכלי להגיע למשבצת היעד
     static constexpr int DEFAULT_TRAVEL_TIME_MS = 1000;
    // שעון התחלת המשחק
    static constexpr int INITIAL_TIME_MS = 0;
    
    // קבוע המייצג קואורדינטת לחיצה לא תקפה או היעדר בחירה
    static constexpr int INVALID_COORDINATE = -1;
};