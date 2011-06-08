package pl.com.mcg.client;

import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("testService")
public interface TestService extends RemoteService {
	String login(String username, String password);

	String sayHello();

	String logout();

	String createCustomer(String firstname, String lastname);
}
