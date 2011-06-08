package pl.com.mcg.ejb.impl;

import java.util.Date;

import javax.ejb.Remote;
import javax.ejb.Stateless;
import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;

import pl.com.mcg.ejb.entity.Customer;
import pl.com.mcg.ejb.remote.StandaloneCustomerServiceRemote;
import pl.com.mcg.ejb.service.CustomerService;

@Stateless
@Remote(StandaloneCustomerServiceRemote.class)
public class StandaloneRemoteCustomerServiceBeanImpl implements CustomerService {
	private static final EntityManagerFactory emf = Persistence.createEntityManagerFactory("ejb");

	private EntityManager createEM() {
		return emf.createEntityManager();
	}

	public String getWelcomeMessage() {
		return "Welcome StandaloneRemoteCustomerServiceBean. Current time: " + new Date();
	}

	public Customer createCustomer(String firstname, String lastname) {
		System.out.println("public Customer createCustomer(String firstname, String lastname)");
		Customer customer = new Customer();
		customer.setFirstname(firstname);
		customer.setLastname(lastname);
		EntityManager em = null;
		try {
			em = createEM();

			em.getTransaction().begin();
			em.persist(customer);
			em.getTransaction().commit();
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			if (em != null && em.isOpen())
				em.close();
		}

		return customer;
	}

	public String getCustomerFullname(Long id) {
		EntityManager em = null;
		try {
			em = createEM();

			Customer customer = em.find(Customer.class, id);
			if (customer != null) {
				return customer.getLastname() + " " + customer.getFirstname();
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			if (em != null && em.isOpen())
				em.close();
		}

		return null;
	}
}