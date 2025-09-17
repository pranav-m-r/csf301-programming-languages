// import java.util.Date;
// import java.sql.Date;

public class DateDemo {
    public static void demonstrateDifferentDateClasses() {
        java.util.Date utilDate = new java.util.Date();
        java.sql.Date sqlDate = new java.sql.Date(System.currentTimeMillis());
        
        System.out.println("java.util.Date: " + utilDate.getClass().getName());
        System.out.println("java.sql.Date: " + sqlDate.getClass().getName());
        
        System.out.println("Are they the same class? " + 
                          (utilDate.getClass() == sqlDate.getClass()));
        
        System.out.println("util.Date toString(): " + utilDate.toString());
        System.out.println("sql.Date toString(): " + sqlDate.toString());
        
        System.out.println("util.Date has time: " + utilDate.getTime());
        System.out.println("sql.Date valueOf: " + java.sql.Date.valueOf("2024-01-01"));
    }
    
    public static void main(String[] args) {
        demonstrateDifferentDateClasses();
    }
}