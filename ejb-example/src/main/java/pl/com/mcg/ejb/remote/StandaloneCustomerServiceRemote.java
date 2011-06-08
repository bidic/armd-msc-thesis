package pl.com.mcg.ejb.remote;

import javax.ejb.Remote;

import pl.com.mcg.ejb.service.CustomerService;

@Remote
public interface StandaloneCustomerServiceRemote extends CustomerService {
}