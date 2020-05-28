﻿#include "param.h"

namespace Param{
    namespace Field{

        double PITCH_LENGTH = 12000;                 // 场地长
        double PITCH_WIDTH = 9000;                  // 场地宽
        double PITCH_MARGIN = 10;                   // 边界宽度
        double CENTER_CIRCLE_R = 500;               // 中圈半径

        double GOAL_POST_AVOID_LENGTH = 20;         //伸进场地内门柱的避障长度
        double GOAL_POST_THICKNESS = 20;           //门柱宽度
        bool   IF_USE_ELLIPSE = false;             // whether use ellipse penalty
        double PENALTY_AREA_WIDTH = 2400;           // rectangle禁区宽度
        double PENALTY_AREA_DEPTH = 1200;            // rectangle禁区深度
        double PENALTY_AREA_R = 800;               // ellipse penalty 两个圆弧
        double PENALTY_AREA_L = 350;          // ellipse penalty 连接两个圆弧的线段
        double PENALTY_L = 500;						//代替PENALTY_AREA_L
        double PENALTY_MARK_X = 4800;               // 点球点的X坐标
        double OUTER_PENALTY_AREA_WIDTH = 1950;     // 外围禁区宽度(界外开球时不能站在该线内)
        double FREE_KICK_AVOID_BALL_DIST = 500;     // 开任意球的时候,对方必须离球这么远
//        double FIELD_WALL_DIST = 20;               // 场地护栏到边界的距离
        double GOAL_WIDTH = 1200;
        double GOAL_DEPTH = 200;
        double RATIO = 15;
    }
    namespace Rule{
        const int Version = 2019; // 规则的版本/年份
        const double MaxDribbleDist = 500; // 最大带球距离, 0代表没有限制
    }
    namespace Vehicle{
        namespace V2{
            const double PLAYER_SIZE = 90;
            const double PLAYER_FRONT_TO_CENTER = 80;
            const double PLAYER_CENTER_TO_BALL_CENTER = 93;
            const double KICK_ANGLE = ::Param::Math::PI*30/180; // 可以击球的最大相对身体角度
            const double DRIBBLE_SIZE = PLAYER_FRONT_TO_CENTER + ::Param::Field::BALL_SIZE; // 带球时离球的距离
            const double DRIBBLE_ANGLE = ::Param::Math::PI*17/180; // 可以带球的最大相对身体角度
            const double HEAD_ANGLE = 57*Param::Math::PI/180; // 前面的开口角度
            //const double TOUCH_SHIFT_DIST = 10.06;		// Touch时后退的距离
            const double TOUCH_SHIFT_DIST = 93;
        }
    }
}
