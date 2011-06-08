package pl.com.mcg.ejb.service;

import pl.com.mcg.ejb.entity.Customer;

public interface CustomerService {
	String getWelcomeMessage();

	Customer createCustomer(String firstname, String lastname);

	String getCustomerFullname(Long id);
}