package pl.com.mcg.ejb.impl;

import java.util.Date;

import javax.ejb.Remote;
import javax.ejb.Stateless;
import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;

import pl.com.mcg.ejb.entity.Customer;
import pl.com.mcg.ejb.remote.CustomerServiceRemote;
import pl.com.mcg.ejb.service.CustomerService;

@Stateless
@Remote(CustomerServiceRemote.class)
public class RemoteCustomerServiceBeanImpl implements CustomerService {

	@PersistenceContext(unitName = "gf-ejb")
	EntityManager em;

	public String getWelcomeMessage() {
		return "Welcome RemoteCustomerServiceBean. Current time: " + new Date();
	}

	public Customer createCustomer(String firstname, String lastname) {
		System.out.println("public Customer createCustomer(String firstname, String lastname)");
		Customer customer = new Customer();
		customer.setFirstname(firstname);
		customer.setLastname(lastname);
		try {
			em.persist(customer);
		} catch (Exception e) {
			e.printStackTrace();
		}

		return customer;
	}

	public String getCustomerFullname(Long id) {
		Customer customer = em.find(Customer.class, id);
		if (customer != null) {
			return customer.getLastname() + " " + customer.getFirstname();
		}
		return null;
	}
}