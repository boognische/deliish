Commerce Postcode Filter
------------------------
Postal code filtering functionality for the Drupal Commerce checkout process.
This module provides 2 main modes (whitelisting and blacklisting) which allow
shop owners to restrict the areas of a country that they deliver to. The admin
form can be found at /admin/commerce/config/postal-code-filter

Blacklisting
------------
If a customer tries to complete checkout with a postcode that is on the
blacklist, they will be prevented from doing so. This allows shop owners to
exclude a certain part of the country.

Whitelisting
------------
If a customer tries to complete checkout with a postcode that is not on the
whitelist, they will be prevented from doing so. This allows shop owners to
include only a small part of the country (e.g. they may wish only to accept
orders from their locality. A whitelist allows them to do so without needing
to exhaustively list all the postal codes that they do not accept)

Partial matches
---------------
Matches in both modes are done on partial postcodes, so the blacklist could
contain "PL" which would prevent the postcode PL5 4AB from being submitted.
Similarly you could <em>whitelist</em> only the "PL" postal code, which would
prevent the postcode "BS3 4BC" from being submitted.

Postcode checking block
-----------------------
The core module exposes a postcode checking block allowing customers to check
their postcode before proceeding to the checkout.

Features integration
--------------------
All module config is stored in a single variable called
"commerce_postal_code_filter_config". Features integration is therefore
afforded by use of the Strongarm and Features modules.
(http://drupal.org/project/strongarm + http://drupal.org/project/features)

Billing vs Shipping
-------------------
By default Drupal Commerce only has a "billing" address on the checkout. This
module supports filtering on this address field. In addition, if
commerce_shipping is enabled, filtering will be available on the shipping
address.

Filtering by shipping address requires commerce_shipping-2.x
(http://drupal.org/project/commerce_shipping).

This module depends on Drupal Commerce.
