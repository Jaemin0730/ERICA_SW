package com.assignment.wk11;

import java.awt.Color;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.sql.Statement;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.border.LineBorder;
import javax.swing.border.TitledBorder;

/**
 * =========================
 * Week11 Assignment
 * CSE3010 Database
 * Fall, 2022
 * =========================
 * 
 * -----------
 * Description
 * -----------
 * 
 * This is the skeleton code for the week11 assignment.
 * 
 * -------
 * Grading
 * -------
 * 
 * Execute .java files submitted by students by using
 * the eclipse environment.
 * 
 * For example, if the file 'Week11Submit.java'
 * is submitted by a student, then place it into
 * a package 'com.assignment.wk11' under the project 
 * 'PostgreSQLJDBC'. Then, see if it is compiled and run
 * without any error. 
 * 
 * Then, compare the console output of 'Week11Submit.java' 
 * with the console output of the solution code.
 * 
 * Total: 4 points
 * 1 point for each task (no partial mark)
 * 0 if compilation fails.
 * 
 * @author - Beom Heyn Kim
 *
 */

public class Week11Submit implements ActionListener {
	
	private String url;
    private String user;
    private String password;
	private Connection conn;
	
	private JFrame frame;
	private JPanel panel;
	private JLabel idLabel;
	private JLabel pwdLabel;
	private JTextField idInput;
	private JPasswordField pwdInput;
	private JButton loginButton;
	
	private JTextArea check_area;
	private JComboBox<String> check_box;
	
	
	String[] univDBTableNames = {
			"classroom",
			"department",
			"course",
			"instructor",
			"section",
			"teaches",
			"student",
			"takes",
			"advisor",
			"time_slot",
			"prereq"
		};
	String[] queries = {
			"SELECT * FROM classroom",
			"SELECT * FROM department",
			"SELECT * FROM course",
			"SELECT * FROM instructor",
			"SELECT * FROM section",
			"SELECT * FROM teaches",
			"SELECT * FROM student",
			"SELECT * FROM takes",
			"SELECT * FROM advisor",
			"SELECT * FROM time_slot",
			"SELECT * FROM prereq"
			};
	
	/*
	 * Replace url, user and password, if preferred.
	 */
	public Week11Submit() {
		url = "jdbc:postgresql://localhost/postgres";
		user = "postgres";
		password = "5698";

		frame = new JFrame();
		panel = new JPanel();
		idLabel = new JLabel("ID");
		pwdLabel = new JLabel("Password");
		idInput = new JTextField(user);
		pwdInput = new JPasswordField(password);
		loginButton = new JButton("Login");
		
		panel.setLayout(null);
		
		// Specify location of Components
		idLabel.setBounds(20, 10, 60, 30);
		pwdLabel.setBounds(20, 50, 60, 30);
		idInput.setBounds(100, 10, 80, 30);
		pwdInput.setBounds(100, 50, 80, 30);
		loginButton.setBounds(200, 25,  80, 35);
		
		// Add an ActionListener to the Login Button
		loginButton.addActionListener(this);
		
		// Add Components to Panel
		panel.add(idLabel);
		panel.add(pwdLabel);
		panel.add(idInput);
		panel.add(pwdInput);
		panel.add(loginButton);
		// Add Panel to Frame
		frame.add(panel);
		
		frame.setTitle("Week11 Assignment");					// name on the top of the frame
		frame.setSize(320, 130);								// size of the frame
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);	// call System.exit() on closing
		frame.setVisible(true);									// display the frame on the screen
	}
	
	/*
	 * Task 1: Modify this method.
	 * 
	 * This method pops up University Info window
	 * replacing Login window.
	 * University Info window must be shown after 
	 * the user successfully log in (and the 
	 * connection with the database server is
	 * successfully established).
	 * 
	 */
	private void universityInfo() {
		check_area = new JTextArea();
		check_box = new JComboBox<String>();
		
		frame.setVisible(true);
		
		frame = new JFrame();
		panel = new JPanel();
		
		frame.setLocation(400, 0);
		
		panel.setFont(new Font(null, 1, 12));
		panel.setBorder(new TitledBorder("Inquiry"));
		panel.setBounds(380, 80, 490, 280);
		panel.setLayout(null);
		
		addAllTableNamesToCheckBox();
		
		check_area.setBorder(new LineBorder(Color.gray, 2));
		check_area.setEditable(false);
		
		JScrollPane scroll = new JScrollPane();
		scroll.setViewportView(check_area);
		
		check_box.setBounds(20, 40, 120, 30);
		scroll.setBounds(10, 80, 460, 270);
		
		check_box.addActionListener(this);
		
		panel.add(check_box);
		panel.add(scroll);
		
		frame.add(panel);
		
		frame.setTitle("University Info");              
		frame.setSize(500, 400);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}
	
	/*
	 * Task 2: Implement this method.
	 * 
	 * This method gets table names in University DB 
	 * and add those to the check box.
	 * University DB's relations are hard-coded and
	 * can be found in univDBTableNames
	 * 
	 */
	private void addAllTableNamesToCheckBox() {
		// fill in here
		for (int i = 0; i < univDBTableNames.length; i++) {
			check_box.addItem(univDBTableNames[i]);
		}
		// end
	}

	/*
	 * This method pops up University Info window
	 * alongside the Login window. 
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
		if (e.getSource() == loginButton) {
			// Get the text entered in JTextField and JPasswordField
			user = idInput.getText();
			password = new String(pwdInput.getPassword());
			
			conn = this.connect();
			if (conn != null) {
				universityInfo();
			}
		} else if (e.getSource() == check_box) {
			try {
				check_area.setText("");
				showTable();
			} catch (SQLException se) {
				se.printStackTrace();
			}
		}
	}
	
	
	/*
	 * Task 3: Implement this method.
	 * 
	 * Depending on the given tn, this method gets a right
	 * SQL query hard-coded in queries above.
	 * 
	 */ 
	private String getQuery(String tn) {
		// fill in here
		String s = null;
		for (int i = 0; i < univDBTableNames.length; i++) {
			if (tn.equalsIgnoreCase(univDBTableNames[i])) {
				s = queries[i];
			}
		}
		return s;
		// end
	}
	
	/*
	 * Task 4: Implement this method.
	 * 
	 * This method displays table contents in
	 * the University Info window's JTextArea.
	 * 
	 * First row of the displayed table content
	 * must be the name of columns separated by
	 * a tab ('\t').
	 * 
	 * Second row of the displayed table content
	 * must be dash (-) as many times as the sum
	 * of length of column name string times 3.
	 * (# of dash = length of all column names * 3)
	 * 
	 * Then, each row of the table should be 
	 * displayed line by line.
	 * 
	 * The display should be as follow:
	 * <Column names separated by a tab>
	 * ----------------------------------- 
	 * <The 1st row of the chosen table>
	 * <The 2nd row of the chosen table>
	 * ...
	 * <The last row of the chosen table>
	 *  
	 */
	private void showTable() throws SQLException {
		
		// fill in here
		String s = check_box.getSelectedItem().toString();
		String SQL = getQuery(s);
		// 쿼리문 받아오기
		Statement st = conn.createStatement();
		ResultSet rs = st.executeQuery(SQL);
		ResultSetMetaData dmd = rs.getMetaData();
		for (int i =1; i <= dmd.getColumnCount(); i++) {
			check_area.append(dmd.getColumnName(i) + '\t');
		}
		check_area.append("\n");
		for(int i=1; i<= dmd.getColumnCount(); i++) {
			check_area.append("---------------------");
		}
		check_area.append("\n");
		// 기본 세팅 출력 완료
		while (rs.next()) {
		    for (int i = 1; i <= dmd.getColumnCount(); i++) {
		        check_area.append(rs.getString(i) + '\t');
		    }
		    check_area.append("\n");
		}
		// end
		
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
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        new Week11Submit();
        
    }

}
