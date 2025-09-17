public class ArrayBoundsJava {
    public static void testJavaArrayBounds() {
        int[][] a = new int[11][3];
        
        System.out.println("Array declared as int[][] a = new int[11][3]");
        System.out.println("Valid indices: a[0][0] to a[10][2]");
        
        try {
            System.out.println("Executing: a[12][1] = 1;");
            a[12][1] = 1;
        } catch (ArrayIndexOutOfBoundsException e) {
            System.out.println("Exception caught: " + e.getMessage());
            System.out.println("Exception type: ArrayIndexOutOfBoundsException");
        }
        
        System.out.println("Array remains unchanged after exception");
        for(int i = 0; i < a.length; i++) {
            for(int j = 0; j < a[i].length; j++) {
                if(a[i][j] != 0) {
                    System.out.println("a[" + i + "][" + j + "] = " + a[i][j]);
                }
            }
        }
        System.out.println("All elements remain 0");
    }
    
    public static void main(String[] args) {
        testJavaArrayBounds();
    }
}