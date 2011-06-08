package pl.com.mcg.ejb.entity;

import java.io.Serializable;

import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.TableGenerator;

@Entity
public class Customer implements Serializable {
	private static final long serialVersionUID = -6372656296342116638L;

	@Id
	@TableGenerator(name = "tablegen", table = "ID_TABLE", pkColumnName = "table_name", valueColumnName = "NEXT_ID", initialValue = 2, allocationSize = 1)
	@GeneratedValue(strategy = GenerationType.TABLE, generator = "tablegen")
	private Long id;
	private String firstname;
	private String lastname;

	public void setFirstname(String firstname) {
		this.firstname = firstname;
	}

	public String getFirstname() {
		return firstname;
	}

	public void setLastname(String lastname) {
		this.lastname = lastname;
	}

	public String getLastname() {
		return lastname;
	}

	public void setId(Long id) {
		this.id = id;
	}

	public Long getId() {
		return id;
	}

	@Override
	public String toString() {
		return "#" + id + " - " + lastname + " " + firstname;
	}

}
