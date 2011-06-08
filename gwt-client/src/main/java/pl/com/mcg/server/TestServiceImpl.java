package pl.com.mcg.server;

import javax.naming.Context;
import javax.naming.NamingException;
import javax.servlet.http.HttpSession;

import pl.com.mcg.client.TestService;
import pl.com.mcg.ejb.entity.Customer;
import pl.com.mcg.ejb.remote.AuthorizationServiceRemote;
import pl.com.mcg.ejb.remote.StandaloneCustomerServiceRemote;
import pl.com.mcg.ejb.service.AuthorizationService;

/**
 * The server side implementation of the RPC service.
 */
@SuppressWarnings("serial")
public class TestServiceImpl extends AbstractServiceServlet implements TestService {

	// private void logoutSession() {
	// HttpSession session = getThreadLocalRequest().getSession();
	// ProgrammaticLogin loginCtx = (ProgrammaticLogin) session.getAttribute("login");
	// loginCtx.logout();
	// }

	@Override
	public String login(String username, String password) {
		System.out.println("---------- String login(String username, String password)");
		int level = 0;

		try {
			HttpSession session = getThreadLocalRequest().getSession();
			session.setAttribute("username", username);
			session.setAttribute("password", password);

			AuthorizationService authService = (AuthorizationService) getAuthorizedContext().lookup(
					AuthorizationServiceRemote.class.getName());

			try {
				System.out.println("---------- level 0: " + authService.getAnonymousWelcome());
				level++;
			} catch (Exception e) {
				e.printStackTrace();
			}

			try {
				System.out.println("---------- level 1: " + authService.getUserWelcome());
				level++;
			} catch (Exception e) {
				e.printStackTrace();
			}

			try {
				System.out.println("---------- level 2: " + authService.getAdminWelcome());
				level++;
			} catch (Exception e) {
				e.printStackTrace();
			}

		} catch (Exception e) {
			e.printStackTrace();
		}

		return "AccessLevel: " + level;
	}

	@Override
	public String logout() {
		HttpSession session = getThreadLocalRequest().getSession();
		session.setAttribute("username", null);
		session.setAttribute("password", null);

		return "Logout completed";
	}

	@Override
	public String sayHello() {
		System.out.println("---------- String sayHello()");
		Context ctx = getAuthorizedContext();

		String result = "";

		try {
			AuthorizationService authService = (AuthorizationService) ctx.lookup(AuthorizationServiceRemote.class.getName());

			try {
				result = authService.getAnonymousWelcome();
			} catch (Exception e) {
			}

			try {
				result = authService.getUserWelcome();
			} catch (Exception e) {
			}

			try {
				result = authService.getAdminWelcome();
			} catch (Exception e) {
			}
		} catch (NamingException e) {
			e.printStackTrace();
		}

		return result;
	}

	@Override
	public String createCustomer(String firstname, String lastname) {
		try {
			StandaloneCustomerServiceRemote customerService = (StandaloneCustomerServiceRemote) getInitialContext().lookup(
					StandaloneCustomerServiceRemote.class.getName());
			// CustomerServiceRemote customerService = (CustomerServiceRemote) getInitialContext().lookup(
			// CustomerServiceRemote.class.getName());

			Customer c = customerService.createCustomer(firstname, lastname);
			System.out.println("result: " + c);

			return customerService.getCustomerFullname(c.getId());
		} catch (NamingException e) {
			e.printStackTrace();
		}
		return null;
	}

	@Override
	protected <T> T getServiceBean() {
		// TODO Auto-generated method stub
		return null;
	}
}
