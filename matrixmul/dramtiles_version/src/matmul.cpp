/**
 * @file matmul.cpp
 * @author guanshujie fu
 * @brief 
 * @version 0.1
 * @date 2022-05-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// Includes
#include <stdint.h>
#include <hls_stream.h>

/* define */
#define WIDTH       4096
#define HEIGHT      4096
#define TILE_WIDTH  512
#define TILE_HEIGHT 512
#define TILE_ROW    WIDTH/TILE_WIDTH
#define TILE_COL    HEIGHT/TILE_HEIGHT
#define TILE_NUM    TILE_ROW * TILE_COL
#define PARALLEL    0
#define BytesPerNum 4
#define BytesPerKB  1024
#define BytesPerMB  1024*1024


#if PARALLEL
static void loadrow(int32_t* in, hls::stream<int32_t>& inStream, int rowid, int row, int col) {
    for (int i = 0; i < col; i++) {
#pragma HLS LOOP_TRIPCOUNT min = col max = col
        inStream << in[rowid*row + i];
    }
}

static void loadcol(int32_t* in, hls::stream<int32_t>& inStream, int colid, int row, int col) {
    for (int i = 0; i < row; i++) {
#pragma HLS LOOP_TRIPCOUNT min = row max = col
        inStream << in[i*row + colid];
    }
}


static void loadin(int32_t* in, hls::stream<int32_t>& inStream, int row, int col)
{
    /* load all matrix into the stream */
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            inStream << in[i*row + j];
        }
    }
}
#endif



extern "C" {

void matmul(int* matA, int* matB, int* outC, int tile_x, int tile_y)
{

    /* Creat local buffers */
    int32_t A[TILE_HEIGHT][TILE_WIDTH];
    int32_t B[TILE_HEIGHT][TILE_WIDTH];
    int32_t C[TILE_HEIGHT][TILE_WIDTH];
    const int h = 512;
    const int w = 512;

    /* Load matrix from global memory into local buffer */
readA:
    for (int i = 0, r = 0, c = 0; i < TILE_HEIGHT*TILE_WIDTH; i++, c++) {
#pragma HLS LOOP_TRIPCOUNT min = h*w max = h*w
        if (c == TILE_WIDTH) {
            c = 0;
            r++;
        }
        A[r][c] = (int32_t)matA[(tile_x*TILE_HEIGHT)*WIDTH + tile_y*TILE_WIDTH + i];
    }

readB:
    for (int i = 0, r = 0, c = 0; i < TILE_HEIGHT*TILE_WIDTH; i++, c++) {
#pragma HLS LOOP_TRIPCOUNT min = h*w max = h*w
        if (c == TILE_WIDTH) {
            c = 0;
            r++;
        }
        B[r][c] = (int32_t)matB[(tile_x*TILE_HEIGHT)*WIDTH + tile_y*TILE_WIDTH + i];
    }

    /* Multiplication */
calculateC:
    for (int r = 0; r < TILE_HEIGHT; r++) {
#pragma HLS LOOP_TRIPCOUNT min = h max = h
        for (int c = 0; c < TILE_WIDTH; c++) {
            int32_t res = 0;
    #pragma HLS LOOP_TRIPCOUNT min = w max = w
            for (int i = 0; i < TILE_HEIGHT; i++) {
        #pragma HLS LOOP_TRIPCOUNT min = h max = h
                res += A[r][i] * B[i][c];
            }  
            C[r][c] = res;
        }
    }

    /* Load matrix from local buffer into global memory */
writeC:
    for (int i = 0, r = 0, c = 0; i < TILE_HEIGHT*TILE_WIDTH; i++, c++) {
#pragma HLS LOOP_TRIPCOUNT min = h*w max = h*w
        if (c == TILE_WIDTH) {
            c = 0;
            r++;
        }
        outC[(tile_x*TILE_HEIGHT)*WIDTH + tile_y*TILE_WIDTH + i] += (int32_t)C[r][c];
    }

    /*end*/
    return;
}

}