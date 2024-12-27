#include <stdio.h>
double calculate_pressure_on_block(int row, int column, double block_weight);
double print_pressure(int row, int column, double block_weight);

int main() {
    
    int N,W;
    scanf("%d %d",&N,&W);
    calculate_pressure_on_block(0, 0, W);
    return 0;
}

double calculate_pressure_on_block(int row, int column, double block_weight){
    
    printf("(%d,%d) %.8lf\n",row,column,print_pressure(row, column, block_weight));
    if (row == 0){
        return calculate_pressure_on_block(1, 0, block_weight);
    }
    else if (row == column){
        return calculate_pressure_on_block(row+1, 0, block_weight);
    }

    else {
        return calculate_pressure_on_block(row, column+1, block_weight);
    }
}
double print_pressure(int row, int column, double block_weight){
    double pressure=0;
    if (row == 0){
        pressure = 0;
    }
    else if (column == 0){
        pressure = block_weight/2 + print_pressure(row-1, 0, block_weight)*0.15;
    }
    else if (column == row){
        pressure = block_weight/2 + print_pressure(row-1, column-1, block_weight)*0.15;
    }
    else {
        pressure = block_weight + print_pressure(row-1, column-1, block_weight)*0.15 + print_pressure(row-1, column, block_weight)*0.15;
    }
    return pressure;
}


/*
 Değerleri tanımla
 ilk satır 1. array
 ikinci satır 2. array
 1. arrayi kullanarak 2. arrayı hesapla
 1. arrayi printle
 2. arrayi 1. arraye tanımla
*/
