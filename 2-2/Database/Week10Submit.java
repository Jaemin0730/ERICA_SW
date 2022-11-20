package com.assignment.wk10;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.JTextField;

/**
 * =========================
 * Week10 Assignment 
 * CSE3010 Database
 * Fall, 2022
 * =========================
 * 
 * -----------
 * Description
 * -----------
 * 
 * This is the skeleton code for the week10 assignment.
 * 
 * -------
 * Grading
 * -------
 * 
 * Execute .java files submitted by students by using
 * the eclipse environment.
 * 
 * For example, if the file 'Week10Submit.java'
 * is submitted by a student, then place it into
 * a package 'com.assignment.wk10' under the project 
 * 'PostgreSQLJDBC'. Then, see if it is compiled and run
 * without any error. 
 * 
 * Then, compare the console output of 'Week10Submit.java' 
 * with the console output of the solution code.
 * 
 * Total: 4 points
 * 1 point for each task (no partial mark)
 * 
 * @author - Beom Heyn Kim
 *
 */
public class Week10Submit implements ActionListener {

    private String url;
    private String user;
    private String password;
	
	private JFrame frame;
	private JPanel panel;
	private JLabel idLabel;
	private JLabel pwdLabel;
	private JTextField idInput;
	private JPasswordField pwdInput;
	private JButton loginButton;
	private JButton getTablesButton;
	private JLabel statusLabel;
	private JTextField statusMessageLabel;
	
	private Connection conn;
	
	public Week10Submit() {
		
	}
	
	/*
	 * Task 1. 
	 * Implement this method to properly initialize 
	 * GUI components. 
	 */
	public void task1() {
		url = "jdbc:postgresql://localhost/postgres";
		user = "postgres";
		password = "1234";
		
		// fill in here
		frame = new JFrame();
		panel = new JPanel();
		idLabel = new JLabel("ID");
		pwdLabel = new JLabel("Password");
		idInput = new JTextField();
		pwdInput = new JPasswordField();
		loginButton = new JButton("Login");
		getTablesButton = new JButton("Tables");
		statusLabel = new JLabel("Status");
		statusMessageLabel = new JTextField();
		// end
	}
	
	/*
	 * Task 2. 
	 * Implement this method to properly display 
	 * GUI components. 
	 */
	public void task2() {
		// fill in here
		panel.setLayout(null);
		
		idLabel.setBounds(40, 10, 60, 30);
		pwdLabel.setBounds(200, 10, 80, 30);
		idInput.setBounds(40, 50, 120, 30);
		pwdInput.setBounds(200, 50, 120, 30);
		loginButton.setBounds(40, 90, 120, 35);
		getTablesButton.setBounds(200, 90, 120, 35);
		
		statusLabel.setBounds(30, 130, 60, 30);
		statusMessageLabel.setBounds(30, 170, 300, 30);
		
		loginButton.addActionListener(this);
		getTablesButton.addActionListener(this);
		
		panel.add(idLabel);
		panel.add(idInput);
		panel.add(loginButton);
		panel.add(statusLabel);
		panel.add(statusMessageLabel);
		panel.add(pwdLabel);
		panel.add(pwdInput);
		panel.add(getTablesButton);
		
		frame.add(panel);
		
		frame.setTitle("Week10 Assignment");
		frame.setSize(370, 270);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
		// end
	}
	
	/*
	 * Task 3. 
	 * Implement this method to connect to the database 
	 * with the input user ID and password. 
	 */
	public Connection task3() {
		String statusMessageString = "Connected";
		// fill in here
		user = idInput.getText();
		password = new String(pwdInput.getPassword());
		statusMessageLabel.setText(statusMessageString);
		// end
		return this.connect();
	}
	
	/*
	 * Task 4.
	 * Implement this method to retrieve the list of tables
	 * and print out on Console.
	 */
	public void task4() {
		System.out.println("=====");
		System.out.println("Task4");
		System.out.println("=====");	
		String statusMessageString = "Available Tables are printed out on Console";
		try {
			// fill in here
        	try {
				Statement st = conn.createStatement();
				String sql = "SELECT tablename FROM PG_TABLES where schemaname = 'public' order by tablename";
				ResultSet rs = st.executeQuery(sql);
				while (rs.next()) {
		    		System.out.println(rs.getString(1));
		    	}
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				System.out.println(e.getMessage());
			}
			statusMessageLabel.setText(statusMessageString);
			// end
		} catch (NullPointerException ne) {
			ne.printStackTrace();
			statusMessageLabel.setText("NullPointerException: Did you Login?");
		}
		System.out.println("");
		System.out.println("");
	}
    
    /**
     * Connect to the PostgreSQL database
     *
     * @return a Connection object
     */
    public Connection connect() {
        Connection conn = null;
        try {
            conn = DriverManager.getConnection(url, user, password);
            System.out.println("Connected to the PostgreSQL server successfully.");
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }

        return conn;
    }
    
    /**
     * Disconnect from the PostgreSQL database
     *
     */
    public void disconnect(Connection conn) {
        try {
        	conn.close();
            System.out.println("Connection is closed successfully.");
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
    }
    
    @Override
	public void actionPerformed(ActionEvent e) {
    	if (e.getSource() == loginButton) {
    		conn = task3();
    	} else if (e.getSource() == getTablesButton) {
    		task4();
    	}
	}
    
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        Week10Submit sub = new Week10Submit();
        sub.task1();
        sub.task2();

    }

	
}
