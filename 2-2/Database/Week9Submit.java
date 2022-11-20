package com.assignment.wk9;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Properties;
import java.util.Scanner;


/**
 * =========================
 * Week9 Assignment 
 * CSE3010 Database
 * Fall, 2022
 * =========================
 * 
 * -----------
 * Description
 * -----------
 * 
 * This is the skeleton code for the week9 assignment.
 * 
 * -------
 * Grading
 * -------
 * 
 * Execute .java files submitted by students by using
 * the eclipse environment.
 * 
 * For example, if the file 'Week9Submit.java'
 * is submitted by a student, then place it into
 * a package 'com.assignment.wk9' under the project 
 * 'PostgreSQLJDBC'. Then, see if it is compiled and run
 * without any error. 
 * 
 * Then, compare the console output of 'Week9Submit.java' 
 * with the console output of the solution code.
 * 
 * Total: 4 points
 * 1 point for each task (no partial mark)
 * 
 * @author - Beom Heyn Kim
 *
 */
public class Week9Submit {

	// fill in here
	private final String url = "jdbc:postgresql://localhost:5432/postgres";
    private final String user = "postgres";
    private final String password = "5698";
    // end
    
    /**
     * Connect to the PostgreSQL database
     *
     * @return a Connection object
     */
    public Connection task1() {
    	System.out.println("=====");
		System.out.println("Task1");
		System.out.println("=====");	
        Connection conn = null;
        try {
        	Properties props = new Properties();
        	// fill in here
        	props.setProperty("url", url);
        	props.setProperty("user",user);
        	props.setProperty("password", password);
        	// end
            conn = DriverManager.getConnection(url, props);
            System.out.println("Connected to the PostgreSQL server successfully.");
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
        System.out.println("");
		System.out.println("");

        return conn;
    }
    
    public void disconnect(Connection conn) {
        try {
        	conn.close();
            System.out.println("Connection is closed successfully.");
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
    }
    
    public void task2(Connection conn) {
    	System.out.println("=====");
		System.out.println("Task2");
		System.out.println("=====");	

        try {
        	Statement st = null;
        	String sql = null;
        	// fill in here
        	st = conn.createStatement();
        	sql = "SELECT * FROM instructor where salary = (select max(salary) from instructor)";
        	// end
			ResultSet rs = st.executeQuery(sql);
			System.out.println("info of the instructor who is receiving the highest salary:");
			System.out.println(" ID " + "\t"
					+ "  name   " + "\t"
					+ "dept_name" + "\t"
					+ "salary" + "\t");
			System.out.println("----" + "\t"
					+ "--------" + "\t"
					+ "---------" + "\t"
					+ "------");
			displayInstructor(rs);
    		rs.close();
    		st.close();
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
        System.out.println("");
		System.out.println("");
    }
    
    public void task3(Connection conn) {
    	System.out.println("=====");
		System.out.println("Task3");
		System.out.println("=====");	

        try {
        	Statement st;
        	String sql;
        	ResultSet rs = null;
        	st = conn.createStatement();
        	// fill in here
        	sql = "SELECT * FROM instructor where dept_name = (select dept_name from instructor where salary >= all (select salary from instructor))";
        	rs = st.executeQuery(sql);
        	// end
			System.out.println("info of instructors who are in the same department as the instructor receiving the highest salary:");
			System.out.println(" ID " + "\t"
					+ "  name   " + "\t"
					+ "dept_name" + "\t"
					+ "salary" + "\t");
			System.out.println("----" + "\t"
					+ "--------" + "\t"
					+ "---------" + "\t"
					+ "------");
			displayInstructor(rs);
    		rs.close();
    		st.close();
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
        System.out.println("");
		System.out.println("");
    }
    
    public void task4(Connection conn) {
    	System.out.println("=====");
		System.out.println("Task4");
		System.out.println("=====");	

		String deptName = readDeptNameFromStdin();
    	
		try {
			PreparedStatement pstmt = null;
			ResultSet rs;
			String SQL = "SELECT id, name, dept_name, salary "
	    			+ "FROM instructor "
	    			+ "WHERE dept_name = ?";
			// fill in here
			pstmt = conn.prepareStatement(SQL);
			pstmt.setString(1, deptName);
    		// end
    		rs = pstmt.executeQuery();
    		System.out.println("info of instructors in the department chosen by the user:");
			System.out.println(" ID " + "\t"
					+ "  name   " + "\t"
					+ "dept_name" + "\t"
					+ "salary" + "\t");
			System.out.println("----" + "\t"
					+ "--------" + "\t"
					+ "---------" + "\t"
					+ "------");
			displayInstructor(rs);
    		rs.close();
    		pstmt.close();
        } catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		System.out.println("");
		System.out.println("");
    }
    
    private String readDeptNameFromStdin() {
    	String deptName;
    	Scanner in=new Scanner(System.in);
        System.out.println("Enter the department name you want to get info of instructors from:");
        deptName=in.nextLine();
        System.out.println("The department name entered by the user is: " + deptName);
		in.close();
		return deptName;
	}
    
    private void displayInstructor(ResultSet rs) throws SQLException {
    	while (rs.next()) {
    		System.out.println(rs.getString(1) + "\t"   //First Column = "id"
    				+ rs.getString(2) + "\t"            //Second Column = "name"
    				+ rs.getString("dept_name") + "\t\t"
    				+ rs.getBigDecimal("salary"));
    	}
    }
    
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        Week9Submit sol = new Week9Submit();
        Connection conn = sol.task1();
        sol.task2(conn);
        sol.task3(conn);
        sol.task4(conn);
        sol.disconnect(conn);
    }
}

